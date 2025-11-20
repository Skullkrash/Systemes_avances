# II - Service « from scratch »

**1. Création du container**

S'assurer que le service docker est bien démarré.

```cmd
docker build -t tomcat_jenkins .
```

**2. Démarrage du conteneur proposant le service.**

```cmd
docker run -p 8080:8080 tomcat_jenkins
```

Le service devrait être accessible à l'adresse [http://localhost:8080/jenkins](http://localhost:8080/jenkins)

<!-- **1. Préparation de l’environnement**
```cmd
cd TP4
wget https://get.jenkins.io/war-stable/2.528.2/jenkins.war
``` -->
