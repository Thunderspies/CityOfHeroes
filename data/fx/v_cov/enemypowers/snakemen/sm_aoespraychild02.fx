#########################################################
##	
##

FxInfo

LifeSpan	300

##############################################################################
###############################  Spit Spray  #################################
##############################################################################

Condition
	On 	Time
	Time 	33

	Event
		EName	Emitter
		Type	Local
		At	head
		
		BHVR	Behaviors/FX_SM_SpraySpawn.bhvr
		#Geom	TESTING_Target
		#Geom	LocalPivotz
	End
End

###############################  Spit Spray 1  ################################

Condition
	On 	Time
	Time 	73


	Event
		EName	Prime1
		Type	Start
		At	Emitter
		
		BHVR	Behaviors/FX_SM_SprayProjectile.bhvr
		#Geom	TESTING_Target
		Part	:SM_SprayCoreChild02.part
		Magnet	T_Root
		LookAt	T_Root
		
	End
	
	Event
		EName	Spray
		Type	Local
		At	Prime1
		
		#Geom	TESTING_Target
#		Bhvr	Behaviors/FX_SM_SprayStreak.bhvr
		Part	:SM_SprayCoreStreamChild02.part
				
	End

End

##############################  Spit Spray 2  ################################

Condition
	On 	Time
	Time 	77


	Event
		EName	Prime2
		Type	Start
		At	Emitter
		
		BHVR	Behaviors/FX_SM_SprayProjectile.bhvr
		#Geom	TESTING_Target
		Part	:SM_SprayCoreChild02.part
		Magnet	T_Root
		LookAt	T_Root
		
	End
	
	Event
		EName	Spray2
		Type	Local
		At	Prime2
		
		#Geom	TESTING_Target
#		Bhvr	Behaviors/FX_SM_SprayStreak.bhvr
		Part	:SM_SprayCoreStreamChild02.part
				
	End

End



##############################################################################
###############################  Spit Deaths  #################################
##############################################################################





Condition
	On 	Prime1Hit


	Event
		EName	All
		Type	Destroy
		
	End


End


End				