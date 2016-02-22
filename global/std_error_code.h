#ifndef SN_CORELIB_GLOBAL_STD_ERROR_CODE_H
#define SN_CORELIB_GLOBAL_STD_ERROR_CODE_H

#define SN_E_FILE_NOT_EXIST 1
#define SN_E_DIR_NOT_EXIST 2
#define SN_E_API_INVOKE_LEAK_ARGS 3
#define SN_E_PROTOCOL_ILL_FORMAT 4
#define SN_E_EXEC_JS 5
//'E_FILE_NOT_EXIST'                     => array(1, 'file : %s is not exist'),
//      //数组相关的
//      'E_ARRAY_KEYS_NOT_EXIST'               => array(2, 'array do not have keys %s'),
//      'E_APP_NOT_EXIST'                      => array(3, 'App : %s is not exist'),
//      'E_OBJECT_TYPE_ERROR'                  => array(4, 'object type error , must be instance of %s'),
//      'E_ARG_TYPE_ERROR'                     => array(5, 'argument type error, expect %s'),
//      'E_FILE_IS_NOT_REGULAR'                => array(6, 'file : %s is not a regulat file'),
//      'E_FILE_NOT_READABLE'                  => array(7, 'file : %s is not readable'),
//      'E_FILE_NOT_WRITABLE'                  => array(8, 'file : %s is not writable'),
//      'E_DIR_NOT_EXIST'                      => array(9, 'directory : %s is not exist'),
//      'E_NOT_DIR'                            => array(10, '%s is not a directory'),
//      'E_NOT_CALLABLE'                       => array(11, '% is not callable'),
//      'E_PERMISSION_DENIED'                  => array(12, 'operation %s on %s , Permission denied'),
//      'E_DEST_NAME_INVALID'                  => array(13, 'copy dir the destination name : %s is subdir of %s'),
//      'E_CLASS_NOT_EXIST'                    => array(14, 'class %s is not exist'),
//      'E_API_INVOKE_TYPE_NOT_SUPPORT'        => array(15, 'API Invoke Type : %s is not supported'),
//      'E_APP_API_NOT_EXIST'                  => array(16, 'application %s does not have interface %s'),
//      'E_APP_API_DISPATCH_ERROR'             => array(17, 'application dispatch error : %s'),
//      'E_API_PERMISSION_DENY'                => array(18, 'Api invoke permission deny , detail msg : %s'),
//      'E_OPERATE_LOGICAL_ERROR'              => array(19, 'operate logical error : %s'),
//      'E_API_INVOKE_LEAK_ARGS'               => array(20, 'API Invoke need param %s'),
//      'E_APP_AJAX_HANDLER_NOT_EXIST'         => array(21, 'ajax handler method : %s is not exist'),
//      'E_APP_NOT_INSTALLED'                  => array(22, 'APP : %s is not installed in current System'),
//      'E_APP_AJAX_HANDLER_FILE_NOT_EXIST'    => array(23, 'App : %s\'s ajax handler file is not exist'),
//      'E_APP_AJAX_HANDLER_CLASS_NOT_EXIST'   => array(24, 'App : %s\' ajax class : %s not exist'),
//      'E_APP_META_FILE_NOT_EXIST'            => array(25, 'App  : %s meta file not exist'),
//      'E_APP_META_FORMAT_ERROR'              => array(26, 'App : %s meta must be array type, please check'),
//      'E_MODULE_NOT_SUPPORT'                 => array(27, 'App module %s is not exist in WebOS system'),
//      'E_METHOD_NOT_EXIST'                   => array(28, '%s#%s is not exist'),
//      'E_DATATYPE_ERROR'                     => array(29, 'data type error, expect type : %s'),
//      'E_API_AUTH_TARGET_TYPE_NOT_SUPPORTED' => array(30, 'api authorize target type : %d is not supported'),
//      'E_API_CODE_MAP_FILE_NOT_EXIST'        => array(31, '%s invoke\' authorize code file is not exist'),
//      'E_API_AUTHORIZE_CODE_NOT_EXIST'       => array(32, 'Api call %s\' authorize code not exist'),
//      'E_API_CALL_PERMISSION_DENY'           => array(33, 'Sorry, you have no permission to invoke API %s'),
//      'E_API_AUTHORIZE_MAP_NOT_VALID'        => array(34, 'Api authorize code map data not valid, maybe is not a array, or leak map field'),
//      'E_API_SYS_HANDLER_NOT_EXIST'          => array(35, 'Sys API handler %s is not exist'),
//      'E_SYS_NOT_DISPATCHABLE'               => array(36, 'Sys script : %s is not dispatchable'),
//      'E_API_INVOKE_LEAK_META'               => array(37, 'Api invoke leak meta infomation'),
//      'E_DB_OPT_ERROR'                       => array(38, 'data base operation error : %s'),
//      'E_MODULE_NOT_EXIST'                   => array(39, 'target application module is not exist'),
//      'E_APP_ACL_ALREADY_MOUNTED'            => array(40, 'APP %s have registered, if you want to remount, you can invoke reregisterApp method'),
//      'E_APP_ACL_NOT_MOUNTED'                => array(41, 'APP %s\' acl data have not registered'),
//      'E_APP_ACL_FILE_NOT_EXIST'             => array(42, 'App : %s acl data file is not exist'),
//      //语言包相关
//      'E_LANG_FILE_NOT_EXIST'                => array(43, 'lang file : %s is not exist'),
//      'E_LANG_KEY_NOT_EXIST'                 => array(44, 'lang key : %s is not exist'),
//      'E_LANG_TYPE_ERROR'                    => array(45, 'lang data must be array'),
//      'E_MAKE_CACHE_ERROR'                   => array(46, 'make cache object  : %s error : %s'),
//      'E_CONFIG_FILE_NOT_EXIST'              => array(47, 'config file : %s not exist'),
//      'E_VENDER_FRAMEWORK_NAME_EMPTY'        => array(48, 'vender framework name can not be empty'),
//      'E_EXTENSION_NOT_LOADED'               => array(49, 'extension %s is not loaded'),
//      'E_ARGUMENT_ERROR'                      => array(50, 'function invoke argument error : %s'),
//      'E_SPH_DISABLED' => array(51, 'sphinx search server current is disabled'),
//      'E_REQUIRED_CONFIG_KEY_NOT_EXIST'        => array(52, 'require config key s%s is not exist'),
//      'E_API_INVOKE_NEED_LOGIN' => array(53, 'api invoke need login'),
//      'E_APP_MODULE_CANOT_EMPTY'               => array(54, 'App module can not be empty'),
//      'E_HTML_IS_NOT_BUILD' => array(55, 'target html file : %s is not exist, maybe not be generated'),
//      'E_APP_META_FILE_NOT_EXIST' => array(56, 'app meta file is not exist'),

//      'E_API_INVOKE_LEAK_SECURITY' => array(58, 'Api invoke leak security infomation'),
//      'E_MULTI_LOGIN_FAIL' => array(59, 'multi login fail'),
//      'E_AUTH_COOKIE_NOT_EXIST' => array(60, 'auth cookie is not exist')

#define STD_EXCEPTION_CONTEXT QString("sn::corelib::StdErrorType")

#endif // SN_CORELIB_GLOBAL_STD_ERROR_CODE_H
