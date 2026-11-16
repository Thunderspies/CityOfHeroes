#########################################################
## Talsorian Gauntlets - Shockwave
#########################################################

FxInfo

LifeSpan	100

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	33.5

	Event
		EName	Streak
		Type	local
		At	Mystic
		Sound clawtwirl5 100.0 100.0 .7
	End
End

## POWER FX #######################################################

Condition
	On 	Time
	Time 	33.5

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	:Blades_Swoosh.bhvr
		Lifespan	2
	End
End

Condition
	On 	Time
	Time 	38

	Event
		EName	roothookup
		Type	local
		At	root
		Bhvr	behaviors/alpha.bhvr
		Geom	RootToChestAdjustment
	End
	
	Event
		EName	hookup
		Type	local
		At	roothookup
		altpiv  1
		Geom	LungeNode
	End
	
	Event
		EName	Prime
		Type	start
		At	hookup
		Geom	onset
		altpiv	1

		Bhvr	:Blades_Shockwave_ProjectileSpin1.bhvr
		Part1	:Blades_Shockwave_Rings1.part

		Magnet	Target
		LookAt	Target
	End

#	Event
#		EName	Prime1
#		Type	start
#		At	hookup
#		altpiv	2
#		Geom	offset
#
#		Bhvr	behaviors/ClawspinPrjct2.bhvr
#		Part1	:Blades_Shockwave_Rings1.part
#
#		Magnet	Target
#		LookAt	Target
#	End
End

## CLEAN UP #######################################################

Condition
	On 	Time
	Time 	36.5
	
	Event
		EName	streak
		Type	Destroy
	End
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

Condition
	On 	Prime1Hit
	
	Event
		EName 	Prime1
		Type	Destroy
	End
End

#########################################################

End