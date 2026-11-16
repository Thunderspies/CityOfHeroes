#########################################################
##	onfire
##
FxInfo  

#LifeSpan	9900
Flags		InheritAlpha

##########################################################################
###################################################################

Condition

	Event
		Type	start 
		At	Root
		Sound graveldust 90 90 .8
		Part	:OrganicFunk.part
		
	End
	
	Event
		Type	start 
		At	Root
		
		BhvrOverride
			PositionOffset	0 10 0
		End

		Part	:Earthdust.part
		Part	:Earthdust2.part
		LifeSpan	5
		
	End

	Event
		Type	start 
		At	Root
		
#		BhvrOverride
#			PositionOffset	0 10 0
#		End

		Part	:Earthdust.part
		Part	:Earthdust2.part
		LifeSpan	5
		
	End
	
	Event
		Type	start 
		At	Root
		
		BhvrOverride	
			Startjitter	1 3 1
			PositionOffset	0 15 0
			FadeOutLength		100
		End
		
		Part	Explosions\StoneStatueExplosions\RockSparks.part
		Part	Explosions\StoneStatueExplosions\RockExplosionBig.part
		Part	Explosions\StoneStatueExplosions\RockExplosion01.part
		Part	Explosions\StoneStatueExplosions\RockExplosion02.part
		Part	Explosions\StoneStatueExplosions\RockBits.part
		Part	Explosions\StoneStatueExplosions\RockBits2.part
		Sound graniteexplo4 80 80 .9
	
		LifeSpan	5
	End
	
	Event
		Type	start 
		At	Root
		
		BhvrOverride	
			Startjitter	1 3 1
			PositionOffset	0 10 0
			FadeOutLength		100
		End
		
		Part	Explosions\StoneStatueExplosions\RockSparks.part
		Part	Explosions\StoneStatueExplosions\RockExplosionBig.part
		Part	Explosions\StoneStatueExplosions\RockExplosion01.part
		Part	Explosions\StoneStatueExplosions\RockExplosion02.part
		Part	Explosions\StoneStatueExplosions\RockBits.part
		Part	Explosions\StoneStatueExplosions\RockBits2.part
		Sound graniteexplo4 80 80 .9
	
		LifeSpan	5
	End

	Event
		Type	start 
		At	Root
		
		BhvrOverride	
			Startjitter	1 3 1
			PositionOffset	0 5 0
			FadeOutLength		100
		End
		
		Part	Explosions\StoneStatueExplosions\RockSparks.part
		Part	Explosions\StoneStatueExplosions\RockExplosionBig.part
		Part	Explosions\StoneStatueExplosions\RockExplosion01.part
		Part	Explosions\StoneStatueExplosions\RockExplosion02.part
		Part	Explosions\StoneStatueExplosions\RockBits.part
		Part	Explosions\StoneStatueExplosions\RockBits2.part
		Sound graniteexplo4 80 80 .9
	
		LifeSpan	5
	End

End

End			