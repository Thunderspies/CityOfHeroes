#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo
Lifespan 70

#############################################################

Condition
	On 	Time
	Time 	0
	Event
		Type	Local
		At	Root
		Sound 	Maelstrom_Death_01 200 200 .5
	End

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 0.4 0.6
		End
		part	:CoreExplosion_Flash.part
		part	:CoreExplosion_Streaks.part
		Part	:CoreExplosion_Shockwave.part
		Part	:CoreExplosion_Glow.part
		part	:CoreExplosion_Sparks.part

	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 0.4 0.6
		End
		part	:CoreExplosion_Flash.part
		part	:CoreExplosion_Smoke.part
		part	:CoreExplosion_Sparks.part

	End
End
#############################################################

End