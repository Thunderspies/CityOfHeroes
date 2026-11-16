#########################################################
##	FireAura
FxInfo

LifeSpan	200

################################################################3
Condition
	On	Time
	Time	0


	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	:Red_ElectricityGlow3.part
		Part2	:Red_ElectricityGlowStar3.part
		Part3	:Red_ElectricityArch4a.part
		Part4	:Red_ElectricityArchSmalla.part
		Sound lightningbolthit 70 70 .8
		LifeSpan	23
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	:Red_ElectricityArchContinuing.part
		Part2	:Red_ElectricityArchContinuing2.part
		Sound suckup 70 70 .83
		LifeSpan	120
	End
End

Condition
	On	Time
	Time	8.5

	Event
		EName 	Prime
		Type	Start 
		At	chest
		Bhvr	behaviors/ConsumeProjectile.bhvr
		Magnet	T_WepL
		LookAt	T_WepL
		#Part1	:Red_EndurenceFlame.part
		#Part2	:Red_EndurenceFlame2.part
		
		Part4	:Red_ElectricityStar.part
		Part	:Red_ElectricityControl01a.part
		Part	:Red_ElectricityControl02a.part

		Part	:Red_ElectricityBallTaila.part
		Part	:Red_ElectricityBallTail2a.part
		Part	:Red_ElectricityBallTailWhite.part
		Part	:Red_ElectricityBallTail2aWhite.part

		Part	:Red_ElectricityArcha.part
		Part	:Red_ElectricityRinga.part
		Part	:Red_ElectricityArchInverta.part




#		Part	:Red_ElectricityInnerGlowa.part
#		Part	:Red_ElectricityOuterGlowa.part


	End


End

Condition
	On	PrimeHit
	
	Event
		Ename	Prime
		Type	Destroy
	End
	
###########################################################################

	Event
		EName	1
		Type	Local
		At	T_chest
		#part1	Powers/Healing/enduranceBodyGlowsa.part
		part2	Powers/Healing/enduranceBodyGlows2a.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	28
	End

	Event
		EName	2
		Type	Local
		At	T_FootR
		part1	Powers/Healing/enduranceBodyGlowsa.part
		PMagnet T_LLEGR
		LifeSpan	28
	End

	Event
		EName	3
		Type	Local
		At	T_ULEGL
		part1	Powers/Healing/enduranceMotionGlowsa.part
		PMagnet T_LLEGL
		LifeSpan	28

	End

	Event
		EName	4
		Type	Local
		At	T_LLEGL
		part1	Powers/Healing/enduranceMotionGlowsa.part
		PMagnet T_FootL
		LifeSpan	28
	End

	Event
		EName	5		
		Type	Local
		At	T_FootL
		part1	Powers/Healing/enduranceBodyGlowsa.part
		PMagnet T_LLEGL
		LifeSpan	28
	End



	Event
		EName	6
		Type	Local
		At	T_ULEGR
		part1	Powers/Healing/enduranceMotionGlowsa.part
		PMagnet T_LlegR
		LifeSpan	28

	End

	Event
		EName	7
		Type	Local
		At	T_LLEGR
		part1	Powers/Healing/enduranceMotionGlowsa.part
		PMagnet T_FootR
		LifeSpan	28
	End
		
	Event
		EName	9
		Type	Local
		At	T_Hair
		part2	Powers/Healing/enduranceHeadGlowsa.part
		LifeSpan	28
	End

	Event
		EName	10
		Type	Local
		At	T_UARML
		part1	Powers/Healing/enduranceArmMotionGlowsa.part
		#part2	Powers/Healing/enduranceArmMotionGlows2a.part
		PMagnet T_LARML
		LifeSpan	28

	End

	Event
		EName	11
		Type	Local
		At	T_LARML
		part1	Powers/Healing/enduranceArmMotionGlowsa.part
		#part2	Powers/Healing/enduranceArmMotionGlows2a.part
		PMagnet T_WepL
		LifeSpan	28
	End

	Event
		EName	12
		Type	Local
		At	T_WepL
		part1	Powers/Healing/enduranceArmGlowsa.part
		LifeSpan	28
	
	End

	Event
		EName	13
		Type	Local
		At	T_UARMR
		part1	Powers/Healing/enduranceArmMotionGlowsa.part
		#part2	Powers/Healing/enduranceArmMotionGlows2a.part
		PMagnet T_LARMR
		LifeSpan	28

	End

	Event
		EName	14
		Type	Local
		At	T_LARMR
		part1	Powers/Healing/enduranceArmMotionGlowsa.part
		#part2	Powers/Healing/enduranceArmMotionGlows2a.part
		PMagnet T_WepR
		LifeSpan	28
	End

	Event
		EName	15
		Type	Local
		At	T_WepR
		part1	Powers/Healing/enduranceArmGlowsa.part
		LifeSpan	28
	End

End


	
	
End				