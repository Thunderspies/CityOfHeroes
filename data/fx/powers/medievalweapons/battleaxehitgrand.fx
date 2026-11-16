#########################################################
##	template

FxInfo


Lifespan	90

#########################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow
		Type 	Local
		At	Chest
		Part1	Powers/Claws/Clawflash.part
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
		Sound Axe10 80.0 80.0 .89
	End

	Event
		EName 	Mallet
		Type	local 
		At	Chest
		Part1	Powers/MartialArts/MartialArtsHit.part
		Part2	Powers/MartialArts/MartialArtsHit1.part
		Part3	Powers/MartialArts/MartialArtsHit2.part
		Part4	Powers/MartialArts/MartialArtsHitRaysSmall.part
		
	End

	Event
		EName 	HitFX
		Type	local 
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01Sml.part
		Part2	Powers/MartialArts/MAHitStar02Sml.part
		Part3	Powers/MartialArts/MAHitStar03Sml.part
		Part4	Powers/MartialArts/MAHitGlow01Sml.part
		Part5	Powers/MartialArts/MAHitRays01Sml.part
		
		
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
	
	Repeat	8
	RepeatJitter	2
	
	Event
		EName	Sparkup
		Type 	Start
		At	hips
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
		
	End

End


Condition
	On Time 
	Time 0

	Repeat	7
	RepeatJitter	6
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkExplode.fx
		
	End

End

Condition
	On Time 
	Time 2
	
	Repeat	6
	RepeatJitter	4
	
	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
		
	End


End


Condition
	On Time 
	Time 0

	Repeat	10
	RepeatJitter	2

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkExplode.fx
	End
			
	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
	End
End
	

End	
