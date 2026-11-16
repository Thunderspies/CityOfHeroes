#########################################################
## Talsorian Gauntlet - Spin (Initial Delayed)
########################################################

FxInfo

## SOUND FX ######################################################

Condition
	On 	Time
	Time 	34

	Event
		EName	One
		Type	local
		At	WepR
		Sound Clawspin 100 100 1.0
	End
End

## SWOOSH ######################################################

Condition
	On 	Time
	Time 	34

	Event
		EName	One
		Type	local
		At	WepR
		Part1	:Blades_Spin_Streaks1.part
	End
End

Condition
	On 	Time
	Time 	53

	Event
		EName	Two
		Type	local
		At	WepR
		part1	:Blades_Hit_Embers1.part
		part2	:Blades_Hit_EmbersBroad1.part
	End
End

Condition
	On 	Time
	Time 	54

	Event
		EName	ImpactPoint
		Type	Local
		At	Root
		
		BhvrOverride
			PositionOffset 	-0.75 0 3.5
		End
	End

	Event
		EName	crack
		Type	start
		At	ImpactPoint
		
		Bhvr	:Blades_ImpactCrackSplat.bhvr
		Splat	ImpactCracks.tga
		Lifespan	90
	End

	Event
		EName	Two
		Type	start
		At	ImpactPoint
		Part5	:Blades_Spin_Ring1.part
		Lifespan	2
	End
End

## CLEAN UP ######################################################

Condition
	On Time 
	Time 57
	Event
		EName	One
		Type 	Destroy
	End
End

Condition
	On Time 
	Time 69
	
	Event
		EName	Two
		Type 	Destroy
	End
End

Condition
	On Time 
	Time 94
	
	Event
		EName	all
		Type 	Destroy
	End
End

########################################################

End