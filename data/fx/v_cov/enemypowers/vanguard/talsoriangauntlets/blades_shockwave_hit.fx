#########################################################
## Talsorian Gauntlets - Shockwave Hit FX
#########################################################

FxInfo

Lifespan	90

## SOUND FX #######################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow
		Type 	Local
		At	Chest
		Sound FocusHit2 100.0 100.0 .8
	End
End

## HIT FLASH #######################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow
		Type 	Local
		At	Chest
		ChildFX	:Blades_HitFlash.fx
	End
End

## PHYSICS PARTICLES #######################################################

Condition
	On Time 
	Time 0
	
	Repeat	5
	RepeatJitter	3
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	:Sparks_Explode1.fx
	End
		
	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	:Sparks_Out1.fx
	End
End

Condition
	On Time 
	Time 0

	Chance	.9
	Repeat	4
	RepeatJitter	2
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	:Sparks_OutBig1.fx
	End
End

Condition
	On Time 
	Time 2
	
	Chance	.75
	Repeat	4
	RepeatJitter	3
	
	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	:Sparks_Out1.fx
	End
End

## CLEAN UP #######################################################

Condition
	On Time 
	Time 2
	
	Event
		EName	Pow
		Type 	Destroy
	End
End

#########################################################

End	