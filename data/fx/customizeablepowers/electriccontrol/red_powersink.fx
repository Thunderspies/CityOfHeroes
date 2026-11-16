#########################################################
##	FireAura
FxInfo

LifeSpan	200

################################################################3
Condition

	On	Time
	Time	15

	Event
		EName 	Prime
		Type	Start 
		At	wepL
		Bhvr	behaviors/MorterLobbprojectile.bhvr
		Magnet	T_chest
		LookAt	T_chest
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
		Sound elecball2 76 76 .66



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
	
End


	
	
End				