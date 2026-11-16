#########################################################
##	Poison Gas Continue
##
FxInfo



Condition
	Event
		Type	Local 
		At	Head
		Sound gas_loop 50 50 .2
		bhvr	behaviors/soundFade2.bhvr
		Flags	PowerLoopingSound
		LifeSpan 200
	End
End


Condition
	Event
		EName	InitialSwordFire
		Type	Local 
		At	Head
		Part	:GasBurstCloud.part
		Part	:GasBurstCloudFace.part
		#LifeSpan 45
	End
End	


			