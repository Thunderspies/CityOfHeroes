#########################################################
##	jack
##

FxInfo

Lifespan	200

Flags    InheritAlpha


###############  Impaling Sword   #############

Condition
	On 	Time
	Time 	0

	Event
		EName 	Impale
		Type	Local
		At	Chest
		Bhvr	:SwordImpale.bhvr
		
				
	End

	Event
		EName 	Sword
		Type	Local
		At	Impale
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0 -2
		End
		Geom	GEO_WepR_Sword_1
		Lifespan	60
	End

	Event
		EName 	Glow
		Type	Local 
		At	Sword
		POther	Sword
		
		Part	:SwordGlow1.part
		Part	:SwordGlow2.part
		Bhvr	:BladeOffset1.bhvr
		Sound Spearhit1 80.0 80.0 1.0
	End

	Event
		EName	Pow
		Type 	Local
		At	Chest
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part		
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
		Sound SwordSlicer 80.0 80.0 1.0
	End

End

Condition
	On Time 
	Time 2
	
	Event
		EName	Pow
		Type 	Destroy
			
	End

End


Condition
	On Time 
	Time 0
	
	Repeat	5
	RepeatJitter	3
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
		
	End

End

Condition
	On Time 
	Time 0

	Chance	.5
	Repeat	3
	RepeatJitter	1
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUTbig.fx
		
	End

End

Condition
	On Time 
	Time 2
	
	Chance	.75
	Repeat	4
	RepeatJitter	2
	
	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
		
	End


End


End		