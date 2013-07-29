/* GStreamer
 * Copyright (C) 2013 Thiago Santos <thiago.sousa.santos@collabora.com>
 *
 * gst-qa-override-registry.c - QA Override Registry
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <string.h>

#include "gst-qa-override-registry.h"

typedef struct
{
  gchar *name;
  GstQaOverride *override;
} GstQaOverrideRegistryNameEntry;

static GMutex _gst_qa_override_registry_mutex;
static GstQaOverrideRegistry *_registry_default;

#define GST_QA_OVERRIDE_REGISTRY_LOCK(r) g_mutex_lock (&r->mutex)
#define GST_QA_OVERRIDE_REGISTRY_UNLOCK(r) g_mutex_unlock (&r->mutex)

static GstQaOverrideRegistry *
gst_qa_override_registry_new (void)
{
  GstQaOverrideRegistry *reg = g_slice_new0 (GstQaOverrideRegistry);

  g_mutex_init (&reg->mutex);
  g_queue_init (&reg->name_overrides);

  return reg;
}

GstQaOverrideRegistry *
gst_qa_override_registry_get (void)
{
  g_mutex_lock (&_gst_qa_override_registry_mutex);
  if (G_UNLIKELY (!_registry_default)) {
    _registry_default = gst_qa_override_registry_new ();
  }
  g_mutex_unlock (&_gst_qa_override_registry_mutex);

  return _registry_default;
}

void
gst_qa_override_register_by_name (const gchar * name, GstQaOverride * override)
{
  GstQaOverrideRegistry *registry = gst_qa_override_registry_get ();
  GstQaOverrideRegistryNameEntry *entry =
      g_slice_new (GstQaOverrideRegistryNameEntry);

  GST_QA_OVERRIDE_REGISTRY_LOCK (registry);
  entry->name = g_strdup (name);
  entry->override = override;
  g_queue_push_tail (&registry->name_overrides, entry);
  GST_QA_OVERRIDE_REGISTRY_UNLOCK (registry);
}

static void
gst_qa_override_registry_attach_name_overrides_unlocked (GstQaOverrideRegistry *
    registry, GstQaMonitor * monitor)
{
  GstQaOverrideRegistryNameEntry *entry;
  GList *iter;
  const gchar *name;

  name = gst_qa_monitor_get_element_name (monitor);
  for (iter = registry->name_overrides.head; iter; iter = g_list_next (iter)) {
    entry = iter->data;
    if (strcmp (name, entry->name) == 0) {
      gst_qa_monitor_attach_override (monitor, entry->override);
    }
  }
}

void
gst_qa_override_registry_attach_overrides (GstQaMonitor * monitor)
{
  GstQaOverrideRegistry *reg = gst_qa_override_registry_get ();

  GST_QA_OVERRIDE_REGISTRY_LOCK (reg);
  gst_qa_override_registry_attach_name_overrides_unlocked (reg, monitor);
  GST_QA_OVERRIDE_REGISTRY_UNLOCK (reg);
}