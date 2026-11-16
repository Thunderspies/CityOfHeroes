#########################################################
## Rikti Pylon - Swarm Missile (Explosion)	
#########################################################

FxInfo

LifeSpan  60

## EXPLOSION & CLEAN UP #######################################################

Condition
	On	Time
	Time	0
	
	Event
		EName	Explosion1
		Type	Start
		At	Origin
		Part	:Explosion_Flash01.part
		Part	:Explosion_Flash02.part
		Part	:Explosion_Smoke01.part
		Part	:Explosion_Smoke02.part
		Part	:Explosion_Streaks01.part
		Part	:Explosion_Sparks01.part
		Lifespan	4		
	End

	Event
		EName	Shockwave
		Type	Start
		At	Origin
		BhvrOverride
			PyrRotateJitter	180 0 180
		End
		Part	:Explosion_Shockwave01.part
		Lifespan	4		
	End

End

#########################################################

End