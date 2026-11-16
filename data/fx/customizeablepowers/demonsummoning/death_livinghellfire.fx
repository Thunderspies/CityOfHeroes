#############################################################
## Death_Fire_Demonling.fx
#############################################################

FxInfo
LifeSpan 190

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	StartPositOnly
		At	Root
		Flags 	AdoptParentEntity
		Part	:Explosion_Hellfire_Alpha.part
		Part	:Explosion_Hellfire.part
		Part	:Explosion_Hellfire_Sparks.part
		Lifespan 8
	End

End

#############################################################

End