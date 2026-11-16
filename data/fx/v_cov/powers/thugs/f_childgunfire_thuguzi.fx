#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Root
End
 

Flags    InheritAlpha

LifeSpan		300

######################################################################################################
########################################  Emitter Barrel  ############################################
######################################################################################################

Condition
	On	Time
	Time	0

	Event	
		EName	Emitter
		Type	Local
		At	Origin
		BhvrOverride
			behavior
			positionOffset		.771 -.094 .275
		End

#		Geom	Testing_TargetB
	End
End


######################################################################################################
###########################################   s h o t s   ############################################
######################################################################################################

Condition
	On 	Time
	Time 	1

	Event
		EName	Sounds
		Type	local
		At	Emitter
##		Sound	### Needs Sound
		lifespan 20
	End

	Event
		EName	GunFire
		Type	local
		At	Emitter
	
		ChildFX	V_COV\Powers\Thugs\F_ChildGunFlare_ThugUzi.fx
		Lifespan 60
	End

	Event
		EName 	tracer
		Type	local
		At	Emitter
		BhvrOverride
			behavior
			PositionOffset		0 -.1 0
		End

#		Part1	WEAPONFX/TorchflameA.part
		Lifespan 60
	End
End
