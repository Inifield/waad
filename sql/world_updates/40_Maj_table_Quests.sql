UPDATE quests SET RewRepValue1=0 WHERE RewRepFaction1=0;
UPDATE quests SET RewRepValue2=0 WHERE RewRepFaction2=0;
UPDATE quests SET RewRepValue3=0 WHERE RewRepFaction3=0;
UPDATE quests SET RewRepValue4=0 WHERE RewRepFaction4=0;
UPDATE quests SET RewRepValue5=0 WHERE RewRepFaction5=0; 

UPDATE quests SET RewRepValue1=1 WHERE  (RewRepValue1 BETWEEN 1 AND 10)  ;
UPDATE quests SET RewRepValue1=2 WHERE (RewRepValue1 BETWEEN 11 AND 25) ;
UPDATE quests SET RewRepValue1=3 WHERE (RewRepValue1 BETWEEN 26 AND 75) ;
UPDATE quests SET RewRepValue1=4 WHERE (RewRepValue1 BETWEEN 76 AND 150);
UPDATE quests SET RewRepValue1=5 WHERE (RewRepValue1 BETWEEN 151 AND 250);
UPDATE quests SET RewRepValue1=6 WHERE (RewRepValue1 BETWEEN 251 AND 350) ;
UPDATE quests SET RewRepValue1=7 WHERE (RewRepValue1 BETWEEN 351 AND 500) ;
UPDATE quests SET RewRepValue1=8 WHERE (RewRepValue1 > 501);

UPDATE quests SET RewRepValue2=1 WHERE (RewRepValue2 BETWEEN 1 AND 10);
UPDATE quests SET RewRepValue2=2 WHERE (RewRepValue2 BETWEEN 11 AND 25) ;
UPDATE quests SET RewRepValue2=3 WHERE (RewRepValue2 BETWEEN 26 AND 75);
UPDATE quests SET RewRepValue2=4 WHERE (RewRepValue2 BETWEEN 76 AND 150);
UPDATE quests SET RewRepValue2=5 WHERE (RewRepValue2 BETWEEN 151 AND 250) ;
UPDATE quests SET RewRepValue2=6 WHERE (RewRepValue2 BETWEEN 251 AND 350) ;
UPDATE quests SET RewRepValue2=7 WHERE (RewRepValue2 BETWEEN 351 AND 500) ;
UPDATE quests SET RewRepValue2=8 WHERE (RewRepValue2 > 501) ;


UPDATE quests SET RewRepValue3=1 WHERE (RewRepValue3 BETWEEN 1 AND 10);
UPDATE quests SET RewRepValue3=2 WHERE (RewRepValue3 BETWEEN 11 AND 25)  ;
UPDATE quests SET RewRepValue3=3 WHERE (RewRepValue3 BETWEEN 26 AND 75)  ;
UPDATE quests SET RewRepValue3=4 WHERE (RewRepValue3 BETWEEN 76 AND 150) ;
UPDATE quests SET RewRepValue3=5 WHERE (RewRepValue3 BETWEEN 151 AND 250) ;
UPDATE quests SET RewRepValue3=6 WHERE (RewRepValue3 BETWEEN 251 AND 350) ;
UPDATE quests SET RewRepValue3=7 WHERE (RewRepValue3 BETWEEN 351 AND 500) ;
UPDATE quests SET RewRepValue3=8 WHERE (RewRepValue3 > 501) ;

UPDATE quests SET RewRepValue4=1 WHERE (RewRepValue4 BETWEEN 1 AND 10)  ;
UPDATE quests SET RewRepValue4=2 WHERE (RewRepValue4 BETWEEN 11 AND 25);
UPDATE quests SET RewRepValue4=3 WHERE (RewRepValue4 BETWEEN 26 AND 75);
UPDATE quests SET RewRepValue4=4 WHERE (RewRepValue4 BETWEEN 76 AND 150) ;
UPDATE quests SET RewRepValue4=5 WHERE (RewRepValue4 BETWEEN 151 AND 250);
UPDATE quests SET RewRepValue4=6 WHERE (RewRepValue4 BETWEEN 251 AND 350);
UPDATE quests SET RewRepValue4=7 WHERE (RewRepValue4 BETWEEN 351 AND 500);
UPDATE quests SET RewRepValue4=8 WHERE (RewRepValue4 > 501) ;

UPDATE quests SET RewRepValue5=1 WHERE (RewRepValue5 BETWEEN 1 AND 10)  ;
UPDATE quests SET RewRepValue5=2 WHERE (RewRepValue5 BETWEEN 11 AND 25)  ;
UPDATE quests SET RewRepValue5=3 WHERE (RewRepValue5 BETWEEN 26 AND 75)  ;
UPDATE quests SET RewRepValue5=4 WHERE (RewRepValue5 BETWEEN 76 AND 150) ;
UPDATE quests SET RewRepValue5=5 WHERE (RewRepValue5 BETWEEN 151 AND 250) ;
UPDATE quests SET RewRepValue5=6 WHERE (RewRepValue5 BETWEEN 251 AND 350);
UPDATE quests SET RewRepValue5=7 WHERE (RewRepValue5 BETWEEN 351 AND 500) ;
UPDATE quests SET RewRepValue5=8 WHERE (RewRepValue5 > 501);