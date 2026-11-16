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
	Time 	53


	Event
		EName	Prime1
		Type	Start
		At	Emitter
		
		BHVR	Behaviors/FX_SM_SprayProjectile.bhvr
		#Geom	TESTING_Target
		Part	:SM_SprayCoreChild01.part
		Magnet	T_Root
		LookAt	T_Root
		
	End
	
	Event
		EName	Spray
		Type	Local
		At	Prime1
		
		#Geom	TESTING_Target
#		Bhvr	Behaviors/FX_SM_SprayStreak.bhvr
		Part	:SM_SprayCoreStream.part
				
	End

End

##############################  Spit Spray 2  ################################

Condition
	On 	Time
	Time 	57


	Event
		EName	Prime2
		Type	Start
		At	Emitter
		
		BHVR	Behaviors/FX_SM_SprayProjectile.bhvr
		#Geom	TESTING_Target
		Part	:SM_SprayCoreChild01.part
		Magnet	T_Root
		LookAt	T_Root
		
	End
	
	Event
		EName	Spray2
		Type	Local
		At	Prime2
		
		#Geom	TESTING_Target
#		Bhvr	Behaviors/FX_SM_SprayStreak.bhvr
		Part	:SM_SprayCoreStream.part
				
	End

End


##############################  Spit Spray 3  ################################

Condition
	On 	Time
	Time 	61


	Event
		EName	Prime3
		Type	Start
		At	Emitter
		
		BHVR	Behaviors/FX_SM_SprayProjectile.bhvr
		#Geom	TESTING_Target
		Part	:SM_SprayCoreChild01.part
		Magnet	T_Root
		LookAt	T_Root
		
	End
	
	Event
		EName	Spray3
		Type	Local
		At	Prime3
		
		#Geom	TESTING_Target
#		Bhvr	Behaviors/FX_SM_SprayStreak.bhvr
		Part	:SM_SprayCoreStream.part
				
	End

End


##############################  Spit Spray 4  ################################

Condition
	On 	Time
	Time 	65


	Event
		EName	Prime4
		Type	Start
		At	Emitter
		
		BHVR	Behaviors/FX_SM_SprayProjectile.bhvr
		#Geom	TESTING_Target
		Part	:SM_SprayCoreChild01.part
		Magnet	T_Root
		LookAt	T_Root
		
	End
	
	Event
		EName	Spray4
		Type	Local
		At	Prime4
		
		#Geom	TESTING_Target
#		Bhvr	Behaviors/FX_SM_SprayStreak.bhvr
		Part	:SM_SprayCoreStream.part
				
	End

End



##############################  Spit Spray 5  ################################

Condition
	On 	Time
	Time 	69


	Event
		EName	Prime5
		Type	Start
		At	Emitter
		
		BHVR	Behaviors/FX_SM_SprayProjectile.bhvr
		#Geom	TESTING_Target
		Part	:SM_SprayCoreChild01.part
		Magnet	T_Root
		LookAt	T_Root
		
	End
	
	Event
		EName	Spray5
		Type	Local
		At	Prime5
		
		#Geom	TESTING_Target
#		Bhvr	Behaviors/FX_SM_SprayStreak.bhvr
		Part	:SM_SprayCoreStream.part
				
	End

End

###############################################################################
###########################  Particle Child FX  ###############################
###############################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Cloud
		Type	Local 
		At	Origin

		ChildFx :SM_AOESprayChild02.fx
	End

End



##############################################################################
###############################  Spit Deaths  #################################
##############################################################################







Condition
	On 	Prime1Hit


	Event
		EName	Prime1
		Type	Destroy
		
	End


End

Condition
	On 	Prime2Hit


	Event
		EName	Prime2
		Type	Destroy
		
	End


End

Condition
	On 	Prime3Hit


	Event
		EName	Prime3
		Type	Destroy
		
	End


End

Condition
	On 	Prime4Hit


	Event
		EName	Prime4
		Type	Destroy
		
	End


End

Condition
	On 	Prime5Hit


	Event
		EName	Prime5
		Type	Destroy
		
	End


End

End				