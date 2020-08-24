module.exports = {
  /**
   * BE CAREFUL to run "npm install" before "pm2 start ecosystem.config.js --only <Conversa(Cloud/Local)> --env <enviroment_name>"
   */
  apps : [

    /*
     * CLOUD ENVIROMENT
     */
    {
      name   : "Arqui2RestAPI",
      script : "npm",
      args : "start",
      watch  : false,
      ignore_watch  : ["node_modules"],
      instances: 1,
      env: {
        "NODE_ENV": "prod"
      }
    }
  ]
}