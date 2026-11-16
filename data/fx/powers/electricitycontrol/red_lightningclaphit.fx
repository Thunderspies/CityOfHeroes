#############################################################
## Red_LightningClapHit.fx
#############################################################

FxInfo
LifeSpan	120

#############################################################

Input
	Inpname	Hips
End

#################################################################
##Hands
#################################################################



Condition
	On	Time
	Time	0

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01Sml.part
		Part2	Powers/MartialArts/MAHitStar02Sml.part
		Part3	Powers/MartialArts/MAHitStar03Sml.part
		Part4	Powers/MartialArts/MAHitGlow01Sml.part
		Part5	Powers/MartialArts/MAHitRays01Sml.part
		#Part	Powers/MartialArts/MartialArtsHitRaysSmall.part
		#Sound	elecmeleehit 80.0 30.0 .8
		LifeSpan	10

	End

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	:Red_ElectricityGlow3.part
		Part2	:Red_ElectricityGlowStar3.part
		Part3	:Red_ElectricityArch4a.part
		Part4	:Red_ElectricityArchSmalla.part
		#Sound	pp12 50 30 .7

	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	:Red_ElectricityArchContinuing.part
		Part2	:Red_ElectricityArchContinuing2.part
	End

#############################################################################
##Head dizzy
########################################################

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Part1	:Red_HeadDizzies.part
		Part2	:Red_HeadDizzyRings.part
#		part3	:Red_ElectricityMeleeGlowHit2.part
	End

End

	Condition
	On 	Time
	Time 	23
	Event
		EName 	HandGlow1
		Type	Destroy

	End

End

Condition
	On Time
	Time 5

	Repeat	2

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RedLightningHit.fx

		Lifespan	90

	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RedElectricSpark.fx

		Lifespan	90

	End
End

Condition
	On Time
	Time 2

	Repeat	6

	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RedElectricSpark.fx

		Lifespan	90

	End

	Event
		HardwareOnly
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RedElectricSpark.fx

		Lifespan	90

	End
End


End
