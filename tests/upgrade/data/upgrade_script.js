UpgradeEnv.aa = 123123;
UpgradeEnv.writeLogMsg("xiuxiuxiuxiuxiux");
UpgradeEnv.dbQuery("INSERT INTO `upgrade`.`table1` (`id`, `name`) VALUES (NULL, 'sssssssssss')");
UpgradeEnv.writeLogMsg("cntyasdaghsdjahgs");
var config =  UpgradeEnv.getConfig("Application.config.json");
config.db.host = 1231234342343;
config.zhangxiuchun = "asdasdas";
UpgradeEnv.saveConfig("Application.config.json", config);
UpgradeEnv.saveConfig("xxxx.config.json", config);
asdasgjh
