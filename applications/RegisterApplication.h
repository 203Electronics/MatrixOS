#ifndef REGISTER_APPLICTION_H

#define APPLICATION_HELPER_CLASS_IMPL(CLASS) CLASS##_HELPER
#define APPLICATION_HELPER_CLASS(CLASS) APPLICATION_HELPER_CLASS_IMPL(CLASS)

#define APPLICATION_HELPER_CLASS_OBJ_IMPL(CLASS) CLASS##_HELPER_OBJ
#define APPLICATION_HELPER_CLASS_OBJ(CLASS) APPLICATION_HELPER_CLASS_IMPL(CLASS)

#define APPLICATION_INFO_IMPL(CLASS) CLASS##_INFO
#define APPLICATION_INFO(CLASS) APPLICATION_INFO_IMPL(CLASS)

#define APPLICATION_INDEX 0
inline const uint16_t app_counter_base = __COUNTER__ + 1;
  
#define REGISTER_APPLICTION_H
#endif

#ifndef APPLICATION_VERSION
#define APPLICATION_VERSION 0
#endif

#ifndef APPLICATION_VISIBLITY
#define APPLICATION_VISIBLITY true
#endif
 

extern Application_Info* applications[];

inline Application_Info APPLICATION_INFO(APPLICATION_CLASS) = Application_Info(
                                                                              StaticHash(APPLICATION_AUTHOR "-" APPLICATION_NAME),
                                                                              APPLICATION_NAME,
                                                                              APPLICATION_AUTHOR,
                                                                              APPLICATION_COLOR,
                                                                              APPLICATION_VERSION,
                                                                              []() -> Application*{ return new APPLICATION_CLASS();},
                                                                              APPLICATION_VISIBLITY);

__attribute__ ((__constructor__))
inline void APPLICATION_HELPER_CLASS(APPLICATION_CLASS) (void)
{
  //For some reason this function is called multiple times, so check if application already registered. 
  //TODO: Need to fix
  const uint16_t index = __COUNTER__ - app_counter_base;
  if(applications[index] == &APPLICATION_INFO(APPLICATION_CLASS))
    return;
  applications[index] = &APPLICATION_INFO(APPLICATION_CLASS);
  // ESP_LOGI("APP REG", APPLICATION_NAME " Registered at %d", index);
}

#pragma push_macro( "APPLICATION_INDEX" )
#undef APPLICATION_INDEX
#define APPLICATION_INDEX _Pragma("pop_macro(\"APPLICATION_INDEX\")") APPLICATION_INDEX + 1

#undef APPLICATION_NAME
#undef APPLICATION_AUTHOR
#undef APPLICATION_COLOR
#undef APPLICATION_VERSION
#undef APPLICATION_VISIBLITY
#undef APPLICATION_CLASS
