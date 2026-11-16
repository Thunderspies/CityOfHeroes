#########################################################
##	Fire_Ball
##
FxInfo

Condition

	Event
		EName 	arrow
		Type	start
		At	root
		
		Part	:SonicCoreRingsLight.part
		
		Part	:SonicRingsOutLight.part
		Sound liquify_loop 90 90 .74
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 180
		
	End

	Event
		Type	start 
		At	root
		Part	:Miragez.part	
		Part	:Mirage2z.part	

	End

End

Condition
	
	Event
			
		Type	start
		At	Root
		Bhvr	:LiquifyCameraShake.bhvr
		LifeSpan	1

	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Root
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		45
			PhysForcePower		75
			PhysForcePowerJitter	5
		End

		Lifespan	15

	End
End

End	


			