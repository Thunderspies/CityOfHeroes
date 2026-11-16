#########################################################
##	Fire_Ball
##
FxInfo
##################################

Condition
	On Cycle
	Time 30
	Event
		EName 	Prime
		#At	Origin
		Type	SetLight
		BHVR	GENERIC\LowGlow\lightpulse2.bhvr
		LifeSpan 160
	End

End


Condition
	On	Time
	Time	0
	
	Event
		EName	Pops
		Type	Local
		At	root
		Bhvr	:Offset.bhvr
		Part	:orePops.part
	End


End    

#######################################################################################################3
#####Aura
############################


Condition
	On	Time
	Time	0
	
	Event
		EName	2
		Type	Local
		At	FootR
		part	:Smoke2Sample.part
		part	:Smoke2TendrilSample.part
		part	:oreDust2Sample.part
		Pmagnet	LlegR
		Sound oreGlow_loop 40 40 .33
	End


#LegL


	Event
		EName	4
		Type	Local
		At	LLEGL
		part	:Smoke2Sample.part
		part	:Smoke2TendrilSample.part
		part	:oreDust2Sample.part
		Pmagnet	UlegL
		

	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part	:Smoke2Sample.part
		part	:Smoke2TendrilSample.part
		part	:oreDust2Sample.part
		Pmagnet	LlegL
		
	End
	
	Event
		EName	4
		Type	Local
		At	ULEGL
		part	:Smoke2Sample.part
		part	:Smoke2TendrilSample.part
		part	:oreDust2Sample.part
		Pmagnet	LlegL
		

	End

	Event
		EName	5		
		Type	Local
		At	LlegL
		part	:Smoke2Sample.part
		part	:Smoke2TendrilSample.part
		Pmagnet	FootL
		
	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part	:Smoke2Sample.part
		part	:Smoke2TendrilSample.part
		part	:oreDust2Sample.part
		Pmagnet	LlegR
	End

#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part	:Smoke2Sample.part
		part	:Smoke2TendrilSample.part
		part	:oreDust2Sample.part
		Pmagnet	UlegR
		
	End
	

	Event
		EName	7
		Type	Local
		At	LLEGR
		part	:Smoke2Sample.part
		part	:Smoke2TendrilSample.part
		part	:oreDust2Sample.part
		Pmagnet	FootR
		
	End
	

	Event
		EName	7
		Type	Local
		At	ULEGR
		part	:Smoke2Sample.part
		part	:Smoke2TendrilSample.part
		part	:oreDust2Sample.part
		Pmagnet	LlegR
		
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part	:Smoke1Sample.part
		part	:SmokeTendrilSample.part
		part	:oreDustSample.part
		
	End

		
##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part	:Smoke1Sample.part
		part	:SmokeTendrilSample.part
		part	:oreDustSample.part
		
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part	:Smoke1Sample.part
		part	:SmokeTendrilSample.part
		part	:oreDustSample.part
		
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part	:Smoke1Sample.part
		part	:SmokeTendrilSample.part
		part	:oreDustSample.part
		

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part	:Smoke1Sample.part
		part	:SmokeTendrilSample.part
		part	:oreDustSample.part
		
	End

End    

End	

