#########################################################
##	Fireweapon
########################################################
FxInfo

########################################################

Condition
	On 		TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName 	ChildThruster
		Type	local
		At	Chest
		While	FLY
		Flags	OneAtATime

		ChildFX	ENEMYFX\V_LongBow\LB_Thruster.fx
#		Geom	Testing_TargetB
		Sound Thruster_loop 60 60 .1
	End

End



Condition
	On 		TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName 	ChildThrusterSides
		Type	local
		At	Chest
		While	FLY
		Until	Forward
		Flags	OneAtATime

		ChildFX	ENEMYFX\V_LongBow\LB_ThrusterSides.fx
#		Geom	Testing_TargetB
	End

End




End

#####################################################################################
################################### center jet  #####################################
#####################################################################################

#Condition
#	On	Time
#	Time	1
#
#	Event
#		EName 	JetR
#		Type	local
#		At	Emitter		
#		
#		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRingsEmit.part
#		Part2	ENEMYFX\V_LongBow\LB_P_ThrustFlareCore.part
#		Part3	ENEMYFX\V_LongBow\LB_P_ThrustFlare.part
#		Sound	glow7_loop 70 65 .66	
#	End
#
#	Event
#		EName	FlareRim
#		Type	Local
#		At	Emitter
#		BhvrOverride
#			Behavior
#			PositionOffset 0 -.15 0
#		End
#
#		Part1	ENEMYFX\V_LongBow\LB_P_ThrustRings.part
#	End
#
#	Event
#		EName	FlareRim
#		Type	Local
#		At	Emitter
#		BhvrOverride
#			Behavior
#			PositionOffset 0 -.5 0
#		End
#
#		Part1	ENEMYFX\V_LongBow\LB_P_Smoke.part
#	End
#	
#End
#
#
######################################################################################
###################################    Left jet    ###################################
######################################################################################
#
#
#Condition
#	On	Time
#	Time	1
#
#	Event
#		EName	LeftJet
#		Type	Local
#		At	Chest
#
#		bhvr	ENEMYFX\V_LongBow\LB_B_EmitterLeft.bhvr
#		Geom	Testing_targetB
#	End
#End
#
#Condition
#	On	Time
#	Time	1
#	
#	Event	
#		EName	RightJet
#		Type	Local
#		At	Chest
#
#		bhvr	ENEMYFX\V_LongBow\LB_B_EmitterRight.bhvr
#		Geom	Testing_targetB
#	End
#End	
#
#
#
#
#
#Condition
#	On	Time
#	Time	1
#	DoMany	1
#	
#	Event
#		EName	ChildThrusts
#		Type	Local
#		While	FLY	
#		At	Chest
#		Flags	OneAtATime
#
##		ChildFX	ENEMYFX\V_LongBow\LB_Thruster.fx
#	End
#
#End
#
#
##Condition
##	On	Triggerbits
##	TriggerBits	FLY	
##	#DoMany	1
##
##	Event
##		EName	Thruster
##		Type	Local
##		#While	FLY
##		At	Chest
##
##		ChildFX	ENEMYFX\V_LongBow\LB_Thruster.fx
##		SetState FLY
##	End
##End
##
##Condition
##	On	Triggerbits
##	TriggerBits	FLY	
##
##	Event
##		EName	Thruster
##		Type	SetState
##		Setstate FLY
##	End
##End
#		
#
#
#
#
##Condition
##	On 	triggerbits
##	Triggerbits	death
##	
################## If you want the guy to turn invisible right as he dies AND play particles, do this on triggerbits death
################## (you can't set TicksToLingerAfterDeath to less than about 15 if you want to have a 
################## deathFx because he'll be dead before the fx happens.)
##	Event
##		EName 	Emitter
##		Type	Local
##		At	Origin 
##		Bhvr	behaviors/vanish.bhvr
##		Flags	AdoptParentEntity
##	End
##
##	Event
##		EName 	Emitter
##		Type	Destroy
##	End
##
##	Event
##		EName 	JetL
##		Type	Destroy
##	End
##	
##	Event
##		EName 	JetR
##		Type	Destroy
##	End
##
##	Event
##		EName 	magnet2
##		Type	Destroy
##	End
##
##End
##
#
#End
###################################
#
#