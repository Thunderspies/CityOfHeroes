#########################################################
##	iceweapon
########################################################
FxInfo

LifeSpan	60

Input  
	Inpname	Chest
End

Input  
	Inpname	Head
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	start 
		At	Chest
		Geom	ElectricalMeleeBubble
		bhvr	behaviors/ScaleBubble.bhvr
		
	End

	Event
		EName 	Mallet2
		Type	start 
		At	Chest
		
		Part1	:ElectricityMeleeHitEmber.part
		Part2	:ElectricityMeleeHitStar.part
		Part3	:ElectricityMeleeHitRing.part
		Part4	:ElectricityMeleeHitRing2.part
		Part5	:ElectricityMeleeHitGlows.part
		
		Sound elecmeleehit 100.0 100.0 .8
	End

	Event
		EName 	shock
		Type	start 
		At	Chest
		
		Part1	:ElectricityArchContinuinga.part
		Part2	:ElectricityArchContinuing2a.part
		
	End

	Event
		EName 	shock2
		Type	start 
		At	Chest
		
		Part	:ElectricityArchLarge2.part
		Part	:ElectricityArchLarge.part
		lifeSpan	10		
	End

#############################################################################
##Head dizzy
########################################################

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Part1	:HeadDizzies.part
		Part2	:HeadDizzyRings.part
		part3	:ElectricityMeleeGlowHit2.part
	End

End

####################################################################

Condition
	On 	Time
	Time 	6

	Event
		EName 	Mallet
		Type	Destroy
		
	End

	Event
		EName 	Mallet2
		Type	Destroy
		
	End

End

Condition
	On 	Time
	Time 	36

	Event
		EName 	Core
		Type	Destroy
		
	End

End


Condition
	On Time 
	Time 5

	Repeat	7
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Shock
		ChildFX	V_COV\PhysicsEnabled\RedLightningHit.fx

		Lifespan	90
		
	End

End

Condition
	On Time 
	Time 7

	Repeat	11
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Shock
		ChildFX	V_COV\PhysicsEnabled\RedElectricSpark.fx

		Lifespan	90
		
	End

End


End
