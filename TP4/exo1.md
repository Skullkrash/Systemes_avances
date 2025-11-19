# I – Proposer un service

**1. Récupération de l’image jenkins depuis le hub docker**

```cmd
docker pull jenkins/jenkins:lts-jdk21
```

**2. Démarrage du conteneur proposant le service.**

On lance le conteneur en mappant les ports nécessaires pour le service [Jenkins](https://github.com/jenkinsci/docker/blob/master/README.md)

```cmd
docker run -name jenkins -p 8080:8080 -p 50000:50000 jenkins/jenkins:lts-jdk21
```

> Note : `-name` permet de nommer le conteneur afin de le retrouver plus simplement.

**3. Vérification de la disponibilité du service**

Si aucune erreur n'apparait dans le terminal, le service devrait être démarré et accessible à l'adresse [http://localhost:8080](http://localhost:8080)

**4. Arret du service**

Vu que nous avons nommé le conteneur, nous pouvons simplement l'arreter sans chercher son ID.

```cmd
docker stop jenkins
```
