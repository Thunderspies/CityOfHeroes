#########################################################
## Rikti Pylon - Swarm Missile (Single Missile)	
#########################################################

FxInfo

LifeSpan  90

#########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		ename	curvespot
		Type	local
		At 	origin
		bhvroverride
			positionoffset	 0 8 8
		End
	End

	Event
		ename	curver
		Type	startpositonly
		At 	curvespot
		bhvr	:Missile\Missile.bhvr
		Magnet	Target
	End

	Event
		ename	twister
		Type	local
		At 	curver
		bhvroverride
			positionoffset	12 0 0
		End
		Magnet	Target
	End

	Event
		ename	prime1
		Type	startpositonly
		At 	origin
		bhvroverride
			trackrate	2.8
		End
		#geom	malta_rocket_grey
		part	:Missile\Missile_Trail01.part
		part	:Missile\Missile_Trail02.part
		#part	:Missile\Missile_Glow01.part
		#part	:Missile\Missile_Flare01.part
		Magnet	twister
	End
End

## DUMMY PROJECTILE #######################################################

Condition
	On 	Time
	Time 	0

	Event
		ename	prime
		Type	startpositonly
		At 	origin
		bhvroverride
			trackrate 2.8
		End
		Magnet	Target
	End
End

## EXPLOSION & CLEAN UP #######################################################

Condition
	On 	PrimeHit
	
	Event
		EName 	prime1
		Type	Destroy
	End

	Event
		EName	Explosion1
		Type	startpositonly
		At	prime1
		ChildFX	:Explosion\Missile_Explosion.fx
	End
End

#########################################################

End