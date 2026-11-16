#############################################################
## ChragedBrawlHitNoPhsics.fx
#############################################################

FxInfo
LifeSpan		30

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
		LifeSpan	10

	End

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	:Red_ElectricityGlow3Red.part
		Part2	:Red_ElectricityGlowStar3Red.part
		Part3	:Red_ElectricityArch4aRed.part
		Part4	:Red_ElectricityArchSmallaRed.part
		Part	:Red_ElectricityArchContinuingWhite.part
		Sound elecpunchhit2 50 50 .7
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	:Red_ElectricityArchContinuingRed.part
		Part2	:Red_ElectricityArchContinuing2Red.part
		LifeSpan	20
	End
End

#Condition
#	On Time
#	Time 5
#
#	Repeat	4
#
#	Event
#		EName	Sparkfar
#		Type 	Start
#		At	Chest
#		ChildFX	V_COV\PhysicsEnabled\RedLightningHit.fx
#
#		Lifespan	90
#
#	End
#
#End
#
#Condition
#	On Time
#	Time 2
#
#	Repeat	9
#
#	Event
#		EName	Sparkfar
#		Type 	Start
#		At	Chest
#		ChildFX	V_COV\PhysicsEnabled\RedElectricSpark.fx
#
#		Lifespan	90
#
#	End
#
#End


End
