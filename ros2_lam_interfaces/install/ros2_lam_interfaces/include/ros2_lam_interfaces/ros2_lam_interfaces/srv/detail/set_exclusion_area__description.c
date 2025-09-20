// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from ros2_lam_interfaces:srv/SetExclusionArea.idl
// generated code does not contain a copyright notice

#include "ros2_lam_interfaces/srv/detail/set_exclusion_area__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_ros2_lam_interfaces
const rosidl_type_hash_t *
ros2_lam_interfaces__srv__SetExclusionArea__get_type_hash(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xdd, 0x06, 0x41, 0x2d, 0x1b, 0xf5, 0x89, 0xeb,
      0x57, 0xa0, 0xcc, 0xf9, 0x87, 0x70, 0x65, 0x1b,
      0x3f, 0xf5, 0x4e, 0x74, 0x2d, 0xb3, 0xee, 0x42,
      0x63, 0x1a, 0x25, 0x38, 0xc5, 0x3b, 0x05, 0xf3,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_ros2_lam_interfaces
const rosidl_type_hash_t *
ros2_lam_interfaces__srv__SetExclusionArea_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x61, 0x1c, 0xbd, 0x2c, 0xd2, 0x67, 0x5e, 0xcc,
      0x13, 0x68, 0xe9, 0x64, 0x98, 0x55, 0x95, 0x2c,
      0x2a, 0x47, 0x4d, 0x4c, 0xcf, 0x1b, 0x40, 0xdc,
      0x89, 0xed, 0x3f, 0xe4, 0x9a, 0x18, 0x45, 0x56,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_ros2_lam_interfaces
const rosidl_type_hash_t *
ros2_lam_interfaces__srv__SetExclusionArea_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x88, 0x88, 0xca, 0x5a, 0x2d, 0x2a, 0x24, 0x6b,
      0x31, 0x90, 0x21, 0xa6, 0x1c, 0x71, 0x88, 0x9f,
      0xc0, 0xe2, 0x1c, 0x41, 0x08, 0x1f, 0xed, 0xe1,
      0xe5, 0x88, 0x30, 0x24, 0xa6, 0x07, 0x2f, 0x93,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_ros2_lam_interfaces
const rosidl_type_hash_t *
ros2_lam_interfaces__srv__SetExclusionArea_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xb1, 0x28, 0x74, 0x38, 0x62, 0x86, 0x86, 0x19,
      0xe0, 0xbc, 0xe5, 0xfd, 0x65, 0xb4, 0x8b, 0xd2,
      0xbe, 0xcd, 0xf1, 0xa7, 0x42, 0x0e, 0x19, 0xf8,
      0xbf, 0x06, 0x7d, 0xeb, 0xd3, 0xb2, 0xc6, 0x10,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "service_msgs/msg/detail/service_event_info__functions.h"
#include "builtin_interfaces/msg/detail/time__functions.h"
#include "geometry_msgs/msg/detail/point__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t geometry_msgs__msg__Point__EXPECTED_HASH = {1, {
    0x69, 0x63, 0x08, 0x48, 0x42, 0xa9, 0xb0, 0x44,
    0x94, 0xd6, 0xb2, 0x94, 0x1d, 0x11, 0x44, 0x47,
    0x08, 0xd8, 0x92, 0xda, 0x2f, 0x4b, 0x09, 0x84,
    0x3b, 0x9c, 0x43, 0xf4, 0x2a, 0x7f, 0x68, 0x81,
  }};
static const rosidl_type_hash_t service_msgs__msg__ServiceEventInfo__EXPECTED_HASH = {1, {
    0x41, 0xbc, 0xbb, 0xe0, 0x7a, 0x75, 0xc9, 0xb5,
    0x2b, 0xc9, 0x6b, 0xfd, 0x5c, 0x24, 0xd7, 0xf0,
    0xfc, 0x0a, 0x08, 0xc0, 0xcb, 0x79, 0x21, 0xb3,
    0x37, 0x3c, 0x57, 0x32, 0x34, 0x5a, 0x6f, 0x45,
  }};
#endif

static char ros2_lam_interfaces__srv__SetExclusionArea__TYPE_NAME[] = "ros2_lam_interfaces/srv/SetExclusionArea";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char geometry_msgs__msg__Point__TYPE_NAME[] = "geometry_msgs/msg/Point";
static char ros2_lam_interfaces__srv__SetExclusionArea_Event__TYPE_NAME[] = "ros2_lam_interfaces/srv/SetExclusionArea_Event";
static char ros2_lam_interfaces__srv__SetExclusionArea_Request__TYPE_NAME[] = "ros2_lam_interfaces/srv/SetExclusionArea_Request";
static char ros2_lam_interfaces__srv__SetExclusionArea_Response__TYPE_NAME[] = "ros2_lam_interfaces/srv/SetExclusionArea_Response";
static char service_msgs__msg__ServiceEventInfo__TYPE_NAME[] = "service_msgs/msg/ServiceEventInfo";

// Define type names, field names, and default values
static char ros2_lam_interfaces__srv__SetExclusionArea__FIELD_NAME__request_message[] = "request_message";
static char ros2_lam_interfaces__srv__SetExclusionArea__FIELD_NAME__response_message[] = "response_message";
static char ros2_lam_interfaces__srv__SetExclusionArea__FIELD_NAME__event_message[] = "event_message";

static rosidl_runtime_c__type_description__Field ros2_lam_interfaces__srv__SetExclusionArea__FIELDS[] = {
  {
    {ros2_lam_interfaces__srv__SetExclusionArea__FIELD_NAME__request_message, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {ros2_lam_interfaces__srv__SetExclusionArea_Request__TYPE_NAME, 48, 48},
    },
    {NULL, 0, 0},
  },
  {
    {ros2_lam_interfaces__srv__SetExclusionArea__FIELD_NAME__response_message, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {ros2_lam_interfaces__srv__SetExclusionArea_Response__TYPE_NAME, 49, 49},
    },
    {NULL, 0, 0},
  },
  {
    {ros2_lam_interfaces__srv__SetExclusionArea__FIELD_NAME__event_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {ros2_lam_interfaces__srv__SetExclusionArea_Event__TYPE_NAME, 46, 46},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription ros2_lam_interfaces__srv__SetExclusionArea__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
  {
    {ros2_lam_interfaces__srv__SetExclusionArea_Event__TYPE_NAME, 46, 46},
    {NULL, 0, 0},
  },
  {
    {ros2_lam_interfaces__srv__SetExclusionArea_Request__TYPE_NAME, 48, 48},
    {NULL, 0, 0},
  },
  {
    {ros2_lam_interfaces__srv__SetExclusionArea_Response__TYPE_NAME, 49, 49},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
ros2_lam_interfaces__srv__SetExclusionArea__get_type_description(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {ros2_lam_interfaces__srv__SetExclusionArea__TYPE_NAME, 40, 40},
      {ros2_lam_interfaces__srv__SetExclusionArea__FIELDS, 3, 3},
    },
    {ros2_lam_interfaces__srv__SetExclusionArea__REFERENCED_TYPE_DESCRIPTIONS, 6, 6},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&geometry_msgs__msg__Point__EXPECTED_HASH, geometry_msgs__msg__Point__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = geometry_msgs__msg__Point__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = ros2_lam_interfaces__srv__SetExclusionArea_Event__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[3].fields = ros2_lam_interfaces__srv__SetExclusionArea_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[4].fields = ros2_lam_interfaces__srv__SetExclusionArea_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[5].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char ros2_lam_interfaces__srv__SetExclusionArea_Request__FIELD_NAME__point_list[] = "point_list";

static rosidl_runtime_c__type_description__Field ros2_lam_interfaces__srv__SetExclusionArea_Request__FIELDS[] = {
  {
    {ros2_lam_interfaces__srv__SetExclusionArea_Request__FIELD_NAME__point_list, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription ros2_lam_interfaces__srv__SetExclusionArea_Request__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
ros2_lam_interfaces__srv__SetExclusionArea_Request__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {ros2_lam_interfaces__srv__SetExclusionArea_Request__TYPE_NAME, 48, 48},
      {ros2_lam_interfaces__srv__SetExclusionArea_Request__FIELDS, 1, 1},
    },
    {ros2_lam_interfaces__srv__SetExclusionArea_Request__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&geometry_msgs__msg__Point__EXPECTED_HASH, geometry_msgs__msg__Point__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = geometry_msgs__msg__Point__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char ros2_lam_interfaces__srv__SetExclusionArea_Response__FIELD_NAME__is_ok[] = "is_ok";
static char ros2_lam_interfaces__srv__SetExclusionArea_Response__FIELD_NAME__error_msg[] = "error_msg";

static rosidl_runtime_c__type_description__Field ros2_lam_interfaces__srv__SetExclusionArea_Response__FIELDS[] = {
  {
    {ros2_lam_interfaces__srv__SetExclusionArea_Response__FIELD_NAME__is_ok, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {ros2_lam_interfaces__srv__SetExclusionArea_Response__FIELD_NAME__error_msg, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
ros2_lam_interfaces__srv__SetExclusionArea_Response__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {ros2_lam_interfaces__srv__SetExclusionArea_Response__TYPE_NAME, 49, 49},
      {ros2_lam_interfaces__srv__SetExclusionArea_Response__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char ros2_lam_interfaces__srv__SetExclusionArea_Event__FIELD_NAME__info[] = "info";
static char ros2_lam_interfaces__srv__SetExclusionArea_Event__FIELD_NAME__request[] = "request";
static char ros2_lam_interfaces__srv__SetExclusionArea_Event__FIELD_NAME__response[] = "response";

static rosidl_runtime_c__type_description__Field ros2_lam_interfaces__srv__SetExclusionArea_Event__FIELDS[] = {
  {
    {ros2_lam_interfaces__srv__SetExclusionArea_Event__FIELD_NAME__info, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {ros2_lam_interfaces__srv__SetExclusionArea_Event__FIELD_NAME__request, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {ros2_lam_interfaces__srv__SetExclusionArea_Request__TYPE_NAME, 48, 48},
    },
    {NULL, 0, 0},
  },
  {
    {ros2_lam_interfaces__srv__SetExclusionArea_Event__FIELD_NAME__response, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {ros2_lam_interfaces__srv__SetExclusionArea_Response__TYPE_NAME, 49, 49},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription ros2_lam_interfaces__srv__SetExclusionArea_Event__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {geometry_msgs__msg__Point__TYPE_NAME, 23, 23},
    {NULL, 0, 0},
  },
  {
    {ros2_lam_interfaces__srv__SetExclusionArea_Request__TYPE_NAME, 48, 48},
    {NULL, 0, 0},
  },
  {
    {ros2_lam_interfaces__srv__SetExclusionArea_Response__TYPE_NAME, 49, 49},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
ros2_lam_interfaces__srv__SetExclusionArea_Event__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {ros2_lam_interfaces__srv__SetExclusionArea_Event__TYPE_NAME, 46, 46},
      {ros2_lam_interfaces__srv__SetExclusionArea_Event__FIELDS, 3, 3},
    },
    {ros2_lam_interfaces__srv__SetExclusionArea_Event__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&geometry_msgs__msg__Point__EXPECTED_HASH, geometry_msgs__msg__Point__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = geometry_msgs__msg__Point__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = ros2_lam_interfaces__srv__SetExclusionArea_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[3].fields = ros2_lam_interfaces__srv__SetExclusionArea_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "geometry_msgs/Point[] point_list\n"
  "---\n"
  "bool is_ok\n"
  "string error_msg";

static char srv_encoding[] = "srv";
static char implicit_encoding[] = "implicit";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
ros2_lam_interfaces__srv__SetExclusionArea__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {ros2_lam_interfaces__srv__SetExclusionArea__TYPE_NAME, 40, 40},
    {srv_encoding, 3, 3},
    {toplevel_type_raw_source, 64, 64},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
ros2_lam_interfaces__srv__SetExclusionArea_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {ros2_lam_interfaces__srv__SetExclusionArea_Request__TYPE_NAME, 48, 48},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
ros2_lam_interfaces__srv__SetExclusionArea_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {ros2_lam_interfaces__srv__SetExclusionArea_Response__TYPE_NAME, 49, 49},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
ros2_lam_interfaces__srv__SetExclusionArea_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {ros2_lam_interfaces__srv__SetExclusionArea_Event__TYPE_NAME, 46, 46},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
ros2_lam_interfaces__srv__SetExclusionArea__get_type_description_sources(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[7];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 7, 7};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *ros2_lam_interfaces__srv__SetExclusionArea__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *geometry_msgs__msg__Point__get_individual_type_description_source(NULL);
    sources[3] = *ros2_lam_interfaces__srv__SetExclusionArea_Event__get_individual_type_description_source(NULL);
    sources[4] = *ros2_lam_interfaces__srv__SetExclusionArea_Request__get_individual_type_description_source(NULL);
    sources[5] = *ros2_lam_interfaces__srv__SetExclusionArea_Response__get_individual_type_description_source(NULL);
    sources[6] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
ros2_lam_interfaces__srv__SetExclusionArea_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *ros2_lam_interfaces__srv__SetExclusionArea_Request__get_individual_type_description_source(NULL),
    sources[1] = *geometry_msgs__msg__Point__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
ros2_lam_interfaces__srv__SetExclusionArea_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *ros2_lam_interfaces__srv__SetExclusionArea_Response__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
ros2_lam_interfaces__srv__SetExclusionArea_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *ros2_lam_interfaces__srv__SetExclusionArea_Event__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *geometry_msgs__msg__Point__get_individual_type_description_source(NULL);
    sources[3] = *ros2_lam_interfaces__srv__SetExclusionArea_Request__get_individual_type_description_source(NULL);
    sources[4] = *ros2_lam_interfaces__srv__SetExclusionArea_Response__get_individual_type_description_source(NULL);
    sources[5] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
