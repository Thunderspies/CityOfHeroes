#########################################################
##	Fire_Ball
##
FxInfo
LifeSpan	21

Condition
	On	Time
	Time	0

	Event
		Type Local
		At T_Root
		Sound Sonic3_loop 100 100 .22
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 180
	End
End


Condition
	On 	Time
	Time 	0
		
	Event
			
		Type	local
		At	T_Root
		
		BHVR	:Offset90.bhvr
		Part	:SonicRingElipse.part
		Part	:SonicRingCoreElipse.part
		
		LifeSpan	21

	End
	
	Event
			
		Type	local
		At	T_Root
		
		
		Part	:SonicRingElipse.part
		Part	:SonicRingCoreElipse.part

		LifeSpan	21

	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		60
			PhysForcePowerJitter	10
		End

		Lifespan	21

	End
End

End	


			