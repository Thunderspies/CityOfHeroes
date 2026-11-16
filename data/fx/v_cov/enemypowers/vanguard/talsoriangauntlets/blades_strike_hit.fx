#########################################################
## Talsorian Gauntlets - Strike Hit FX
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
		Sound ClawHit8b 100.0 100.0 .77
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

## CLEAN UP #######################################################

Condition
	On Time 
	Time 2
	
	Event
		EName	Pow
		Type 	Destroy
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
		#ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
		ChildFX		:Sparks_Explode1.fx
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

	Chance	0.5
	Repeat	3
	RepeatJitter	1
	
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
	
	Chance	0.75
	Repeat	4
	RepeatJitter	2
	
	Event
		HardwareOnly
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

EnD