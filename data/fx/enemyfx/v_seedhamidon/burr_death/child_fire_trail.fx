#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo
Lifespan 500

#############################################################

Condition
	On 	Time
	Time 	0
	Chance	0.35
	Event
		Type	Local
		part	:CoreExplosion_Fire_Trail_Small.part
		Lifespan 150
		LifespanJitter	20
	End
End




#############################################################

End