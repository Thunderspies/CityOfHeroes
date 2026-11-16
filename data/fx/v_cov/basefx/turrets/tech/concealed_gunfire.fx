#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Root
End
 

Flags    InheritAlpha

LifeSpan		70

######################################################################################################
########################################  Emitter Barrel  ############################################
######################################################################################################

Condition
	On	Time
	Time	0

	Event	
		EName	Emitter
		Type	Local
		At	WepR
		BhvrOverride
			behavior
			positionOffset		0 .1 .05
		End

#		Geom	Testing_TargetB
	End
End


######################################################################################################
###########################################   s h o t s   ############################################
######################################################################################################

Condition
	On 	Time
	Time 	17

	Event
		EName	lazer
		Type	local
		At	Emitter
		Sound gatgun3 80 80 .8
	End

	Event
		EName	light
		Type	local
		At	lazer
	
		ChildFX	V_COV\BaseFx\turrets\Tech\Concealed_ChildFlare.fx
		Lifespan 54
	End

	Event
		EName 	tracer
		Type	local
		At	Emitter
		BhvrOverride
			behavior
			PositionOffset		0 -.1 0
		End

		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		Lifespan 54
	End
End


End