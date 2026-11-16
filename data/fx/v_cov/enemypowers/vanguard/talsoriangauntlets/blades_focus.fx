#########################################################
## Talsorian Gauntlets - Focus
#########################################################

FxInfo

lifespan	200

#########################################################

Condition

	On 	Time
	Time 	10

	Event
		Type Local
		At origin
		Sound Clawsfocus4 100.0 100.0 .7
	End
End

## SWOOSH AND PROJECTILE #######################################################

Condition
	On 	Time
	Time 	13

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	:Blades_Swoosh.bhvr
		Lifespan	2
	End

	Event
		EName	Prime
		Type	start
		At	root
		Bhvr	behaviors/Focusprojectile1.bhvr
		Part1	:Blades_Focus_Rays1.part
		Magnet	Target
		LookAt	Target
	End
End

## CLEAN UP #######################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#########################################################

End