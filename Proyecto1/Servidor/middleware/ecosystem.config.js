module.exports = {
  /**
   * BE CAREFUL to run "npm install" before "pm2 start ecosystem.config.js --only <Conversa(Cloud/Local)> --env <enviroment_name>"
   */
  apps : [
    /*
     * LOCAL ENVIROMENT
     */
    {
      name   : "Local",
      script : "npm",
      args : "start",
      watch  : true,
      ignore_watch  : ["node_modules"],
      instances: 1,
      env: {
        "NODE_ENV": "dev",
        "URL": "http://localhost:3000"
      }
    },
    /*
     * CLOUD ENVIROMENT
     */
    {
      name   : "Arqui2Middleware",
      script : "npm",
      args : "start",
      watch  : true,
      ignore_watch  : ["node_modules"],
      instances: 1,
      env: {
        "NODE_ENV": "prod",
        "URL": "http://167.99.237.132:3000"
      }
    }
  ]
}