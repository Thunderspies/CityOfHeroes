#########################################################
##	RadiationAura
FxInfo
#
LifeSpan	60
#############################################################

Condition
	On	time
	time	10

	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	.5 0 .5 

		End

		ChildFx	V_COV\BaseFx\SalvageTempPowers\OrganicEnergyUp.fx
		LifeSpan	60

	End
End

Condition

	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	.5 0 .5 

		End

		ChildFx	V_COV\BaseFx\SalvageTempPowers\OrganicEnergyUp.fx
		LifeSpan	60

	End
	
	Event
		Type	start
		At	root
		
		
		Part	:LightstreamEnergy.part

		LifeSpan	40

	End

################################################
##Splats
###################################

	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	-.3 0 2 

		End

		Part	:Lightstream.part
		Splat	Glow.tga

		Bhvr	:Splat.bhvr

		LifeSpan	35
	End

	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	.8 0 2 

		End

		Part	:Lightstream.part
		Splat	Glow.tga

		Bhvr	:Splat.bhvr

		LifeSpan	35
	End


	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	1.6 0 1.6 

		End

		Part	:Lightstream.part
		Splat	Glow.tga

		Bhvr	:Splat.bhvr

		LifeSpan	35
	End


	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	2.15 0 .9

		End

		Part	:Lightstream.part
		Splat	Glow.tga

		Bhvr	:Splat.bhvr

		LifeSpan	35
	End

	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	0 0 -2 

		End

		Part	:Lightstream.part
		Splat	Glow.tga

		Bhvr	:Splat.bhvr

		LifeSpan	35

	End
###########################################################
	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	-1 0 -2 

		End

		Part	:Lightstream.part
		Splat	Glow.tga

		Bhvr	:Splat.bhvr

		LifeSpan	35
	End


	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	-1.7 0 -1.6 

		End

		Part	:Lightstream.part
		Splat	Glow.tga

		Bhvr	:Splat.bhvr

		LifeSpan	35
	End


	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	-2.3 0 -.9

		End

		Part	:Lightstream.part
		Splat	Glow.tga

		Bhvr	:Splat.bhvr

		LifeSpan	35
	End
######################################################
	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	2.2 0 0  

		End

		Part	:Lightstream.part
		Splat	Glow.tga

		Bhvr	:Splat.bhvr

		LifeSpan	35

	End
	
###########################################################
	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	1.8 0 -.9

		End

		Part	:Lightstream.part
		Splat	Glow.tga

		Bhvr	:Splat.bhvr

		LifeSpan	35
	End


	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	1.1 0 -1.7 

		End

		Part	:Lightstream.part
		Splat	Glow.tga

		Bhvr	:Splat.bhvr

		LifeSpan	35
	End

	#############################

	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	-2.4 0 0  

		End

		Part	:Lightstream.part
		Splat	Glow.tga

		Bhvr	:Splat.bhvr

		LifeSpan	35

	End

	#############################

	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	-2 0 1  

		End

		Part	:Lightstream.part
		Splat	Glow.tga

		Bhvr	:Splat.bhvr

		LifeSpan	35

	End
	
	Event
		Type	start
		At	root
		
		BhvrOverride
			PositionOffset	-1.2 0 1.8  

		End

		Part	:Lightstream.part
		Splat	Glow.tga

		Bhvr	:Splat.bhvr

		LifeSpan	35

	End

End

End				