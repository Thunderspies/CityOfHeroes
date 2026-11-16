#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition

	On	Time
	Time	20

	Event
		Ename	P
		Type	startpositOnly
		At	bendmystic
		Sound graniteexplo 88 88 .77

	End

	Event

		Type	local
		At	bendmystic
		
		#Geom	Testing_target
		Part	:PlantSeeds_01.part
		Part	:PlantSeeds_Dark.part
		Part	:PlantSeeds_Black.part
		Part	:PlantSeedDust.part

		LifeSpan	5
	End

End

End	


			