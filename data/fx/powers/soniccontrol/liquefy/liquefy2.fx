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
		Sound liquify_loop 50 50 .37
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 180
		
	End

	Event
		EName 	arrow
		Type	start
		At	root
#		Part	:SonicCoreRingsOut.part
#		Part	:SonicCoreRings.part
#		Part	:SonicRingsOut.part
#		Part	:SonicRingsOut2.part
		LifeSpan	12
	End

	Event
		Type	start 
		At	root
		Part	:Mirage.part	
		Part	:Mirage2.part	
		#Part	:MirageSteam.part

	End

End

Condition
	
	Event
			
		Type	start
		At	Root
		Bhvr	:LiquifyCameraShake.bhvr
		LifeSpan	1

	End

End

End	


			