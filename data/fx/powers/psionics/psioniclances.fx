#########################################################
FxInfo

#####################################################################################

Flags    InheritAlpha

##############################################################################
##	Hands
###########################################################

Condition

	Event
		Type	Local
		At	HandR
		
		Bhvr	:SpikescaleUP.bhvr
		
		Geom	PsionicSpikeRight

		LifeSpan	29

	End

End

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	HandR
		
		Bhvr	:SpikescaleDown.bhvr
		
		Geom	PsionicSpikeRight

		LifeSpan	29

	End

End

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	HandR
		
		ChildFx	:PsionicLanceGeomChildHand.fx
		LifeSpan	60
	End

End

########################################

		
Condition

	Event
		Type	Local
		At	HandL
		
		Bhvr	:ArmTupescaleUP.bhvr
		
		Geom	PsionicSpikeLeft

		LifeSpan	29

	End

End

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	HandL
		
		Bhvr	:ArmTupescaleDown.bhvr
		
		Geom	PsionicSpikeLeft

		LifeSpan	29

	End

End

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	HandL
		
		ChildFx	:PsionicLanceGeomChildHandLeft.fx
		LifeSpan	60
	End

End

########################################

########################### ARM ############################
		
Condition

	Event
		Type	Local
		At	LarmL
		
		Bhvr	:ArmTupescaleUP.bhvr
		
		Geom	PsionicSpikeLarmRight

		LifeSpan	29

	End

End

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	LarmL
		
		Bhvr	:ArmTupescaleDown.bhvr
		
		Geom	PsionicSpikeLarmRight

		LifeSpan	29

	End

End

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	LarmL
		
		ChildFx	:PsionicLanceGeomChildArm.fx
		LifeSpan	60
	End

End

########################### Shoulder ############################

Condition

	Event
		Type	Local
		At	SpadL
		
		BhvrOverride

			PYRrotate	30 0 0
			PositionOffset		-.2 .3 0.05
			#Scale	4 4 4	
		End

		Bhvr	:ShoulderTupescaleUP.bhvr
		
		Geom	PsionicSpikeLeftShoulder

		LifeSpan	35

	End

End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	SpadL
		
		BhvrOverride

			PYRrotate	30 0 0
			PositionOffset		-.2 .3 0.05
			#Scale	4 4 4	
		End

		Bhvr	:ShoulderTupescaleDown.bhvr
		
		Geom	PsionicSpikeLeftShoulder

		LifeSpan	40

	End

End

Condition
	On	Cycle
	Time	70

	Event
		Type	Local
		At	SpadL
		
		ChildFx	:PsionicLanceGeomChild.fx
		LifeSpan	70
	End

End

###################### Back ################################################


Condition

	Event
		Type	Local
		At	Back
		
		Bhvr	:BackTupescaleUP.bhvr
		Geom	PsionicSpikeBack

		LifeSpan	29

	End

End

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	Back
		
		Bhvr	:BackTupescaleDown.bhvr
		Geom	PsionicSpikeBack

		LifeSpan	29

	End

End

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	back
		
		ChildFx	:PsionicLanceGeomChildBack.fx
		LifeSpan	60
	End

End

##################
############################ Psionic Aura ############################
############

Condition

	Event
		Type	Local
		At	head
		
		Part	:PsionicEnergySwirl.part
		Part	:PsionicEnergySwirl_Purple.part
		Part	:PsionicEnergySwirl_Black.part
		Part	:PsionicEnergySwimmies.part
		Part	:PsionicEnergySwimmiesBlue.part

	End	

	Event
		Type	Local
		At	chest
		
		Part	:PsionicEnergySwirlChest.part
		Part	:PsionicEnergySwirlChest_Purple.part
		Part	:PsionicEnergySwirlChest_Black.part
		Part	:PsionicEnergySwimmies.part
		Part	:PsionicEnergySwimmiesBlue.part

	End

##Arms	##################################################################

	Event
		Type	Local
		At	SpadL
		
		Part	:PsionicEnergySwirlLight.part
		Part	:PsionicEnergySwirlLight_Purple.part
		Part	:PsionicEnergySwirlLight_Black.part

	End

	Event
		Type	Local
		At	UArmL
		
		Part	:PsionicEnergySwirl.part
		Part	:PsionicEnergySwirl_Purple.part		
		Part	:PsionicEnergySwirl_Black.part
		Part	:PsionicEnergySwimmies.part
		Part	:PsionicEnergySwimmiesBlue.part
		Pmagnet	LArmL

	End
		
	Event
		Type	Local
		At	LArmL
		
		Part	:PsionicEnergySwirl.part
		Part	:PsionicEnergySwirl_Purple.part
		Part	:PsionicEnergySwirl_Black.part
		Part	:PsionicEnergySwimmies.part
		Part	:PsionicEnergySwimmiesBlue.part
		Pmagnet	HandL

	End
	
	Event
		Type	Local
		At	HandL
		
		Part	:PsionicEnergySwirl.part
		Part	:PsionicEnergySwirl_Purple.part
		Part	:PsionicEnergySwirl_Black.part
		Part	:PsionicEnergySwimmies.part
		Part	:PsionicEnergySwimmiesBlue.part

	End

	Event
		Type	Local
		At	SpadR
		
		Part	:PsionicEnergySwirlLight.part
		Part	:PsionicEnergySwirlLight_Purple.part
		Part	:PsionicEnergySwirlLight_Black.part

	End

	Event
		Type	Local
		At	UArmR
		
		Part	:PsionicEnergySwirl.part
		Part	:PsionicEnergySwirl_Purple.part
		Part	:PsionicEnergySwirl_Black.part
		Part	:PsionicEnergySwimmies.part
		Part	:PsionicEnergySwimmiesBlue.part
		Pmagnet	LArmR

	End
	
	Event
		Type	Local
		At	LArmR
		
		Part	:PsionicEnergySwirl.part
		Part	:PsionicEnergySwirl_Purple.part
		Part	:PsionicEnergySwirl_Black.part
		Part	:PsionicEnergySwimmies.part
		Part	:PsionicEnergySwimmiesBlue.part
		Pmagnet	HandR

	End
		
	Event
		Type	Local
		At	HandR
		
		Part	:PsionicEnergySwirl.part
		Part	:PsionicEnergySwirl_Purple.part
		Part	:PsionicEnergySwirl_Black.part
		Part	:PsionicEnergySwimmies.part
		Part	:PsionicEnergySwimmiesBlue.part

	End
##Legs	##################################################################

	Event
		Type	Local
		At	UlegL
		
		Part	:PsionicEnergySwirl.part
		Part	:PsionicEnergySwirl_Purple.part
		Part	:PsionicEnergySwirl_Black.part
		Part	:PsionicEnergySwimmies.part
		Part	:PsionicEnergySwimmiesBlue.part

	End
	
	
	Event
		Type	Local
		At	UlegR
		
		Part	:PsionicEnergySwirl.part
		Part	:PsionicEnergySwirl_Purple.part
		Part	:PsionicEnergySwirl_Black.part
		Part	:PsionicEnergySwimmies.part
		Part	:PsionicEnergySwimmiesBlue.part

	End
	
	Event
		Type	Local
		At	LlegL
		
		Part	:PsionicEnergySwirl2Left.part
		Part	:PsionicEnergySwirl2Left_Purple.part
		Part	:PsionicEnergySwirl2Left_Black.part
		Part	:PsionicEnergySwimmies.part
		Part	:PsionicEnergySwimmiesBlue.part
		Pmagnet	UlegL

	End
	
	
	Event
		Type	Local
		At	LlegR
		
		Part	:PsionicEnergySwirl2.part
		Part	:PsionicEnergySwirl2_Purple.part
		Part	:PsionicEnergySwirl2_Black.part
		Part	:PsionicEnergySwimmies.part
		Part	:PsionicEnergySwimmiesBlue.part
		Pmagnet	UlegR
	End
End

End




			