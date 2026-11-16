#########################################################
##	onfire
##
FxInfo  

LifeSpan	100
	

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	chest
		Geom	WoodPlank01
		#Geom	WoodPlank02
		#Geom	WoodPlank03
		Bhvr	Behaviors\StartOffset4.bhvr
		#Part1	Powers/FireControl/BurningMeteor.part
		Lifespan	130
	End

End

End			