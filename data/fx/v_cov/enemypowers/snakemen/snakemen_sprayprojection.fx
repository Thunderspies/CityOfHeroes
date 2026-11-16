#########################################################
##	      spray AOE FX
#########################################################

FxInfo

Input  
	InpName	head
End


LifeSpan	100


##############################################################################
###############################  Spit Target  ################################
##############################################################################

Condition
	On	time
	Time	0
	
	Event
		EName	Prime1
		Type	StartPositOnly
		At	Head
	
		bhvr	behaviors\FX_SM_TargetStart.bhvr
		#Geom	Testing_Target
		Magnet	T_Root
		Lifespan	500
	End
End

Condition
	On	Prime1Hit
		
	Event
		EName	TargetNode
		Type	StartPositOnly
		At	Prime1

		bhvr	behaviors\FX_SM_SprayTarget.bhvr
		#Geom	Testing_Target

	End
End





##########################  Spit Prime for hit fx #############################
 
Condition
	On	time
	Time	33
	
	Event
		EName	Prime
		Type	StartPositOnly
		At	Head
		
		bhvr	behaviors\FX_SM_TargetStart.bhvr
		#Geom	Testing_Target
		Magnet	Target
		LookAt	Target
	End
End


Condition
	On	time
	Time	26
	
	Event

		Type	Local
		At	Head
		Sound snakehiss1 80 80 .88
		Sound snakehiss2 80 80 .9
		Sound snakehiss3 80 80 .88
	
	End
End


##############################################################################
###############################  Spit Spray  #################################
##############################################################################

Condition
	On	Time
	Time	31

	Event
		EName	EmitParent
		Type	Local
		At	Head
		bhvrOverride
			behavior
			positionOffset		0 .15 .4
		End
		BHVR	Behaviors/FX_SM_SpraySpawn.bhvr
#		Geom	Testing_TargetB
	End
End



Condition
	On 	Time
	Time 	32

	Event
		EName	Emitter
		Type	Local
		At	EmitParent
		bhvrOverride
			behavior
			spin		-.005 0.0 0.0
		End
		
#		Geom	TESTING_Target
		#Geom	LocalPivotz
	End


End


##############################  Spit Spray 1  ################################

Condition
	On 	Time
	Time 	33


	Event
		EName	Core
		Type	Local
		At	Emitter
		
		BHVR	Behaviors/FX_SM_SprayProjectile.bhvr
		#Geom	TESTING_Target
		Part	:SM_SprayCore.part
		PMagnet	TargetNode
		#LookAt	T_Root
		lifespan 40
		
	End
	
	Event
		EName	Sreak
		Type	Local
		At	Emitter
		
		#Geom	TESTING_Target
		BHVR	Behaviors/FX_SM_SprayProjectile.bhvr
		Part	:SM_SprayCoreStream.part
		PMagnet	TargetNode
		lifespan 40		
	End

End

Condition
	On 	Time
	Time 	32


	Event
		EName	Core01
		Type	Local
		At	Emitter
		
		BHVR	Behaviors/FX_SM_SpraySpawn01.bhvr
		#Geom	TESTING_Target
		Part1	:SM_SprayCoreMeat.part
		Part	:SM_SprayCore01.part
		PMagnet	TargetNode
		#LookAt	T_Root
		lifespan 40
		
	End
End


###############################################################################
##############################  Event Killing  ################################
###############################################################################



End				