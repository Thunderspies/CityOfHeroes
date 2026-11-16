#########################################################
##	
##
FxInfo


Flags    InheritAlpha


##########################################################

Condition

	Event
		EName   Wind
		Type	start 
		At	Root

		Bhvr	:wirlpoolSpinscale.bhvr

		geom	FX_TmpCtrlWisp01
		LifeSpan	50

	End
End

Condition
	On	cycle
	Time	10

	Event
		EName   Wind
		Type	start 
		At	Root

		Bhvr	:wirlpoolSpinscale.bhvr

		geom	FX_TmpCtrlWisp01
		LifeSpan	50
	End
End

##########################################################

Condition

	Event
		EName   Wind
		Type	start 
		At	Root

		Bhvr	:wirlpoolSpinCore.bhvr

		geom	FX_TmpCtrlWisp01
		LifeSpan	50

	End
End

Condition
	On	cycle
	Time	10

	Event
		EName   Wind
		Type	start 
		At	Root

		Bhvr	:wirlpoolSpinCore.bhvr

		geom	FX_TmpCtrlWisp01
		LifeSpan	50
	End
End

##########################################################

Condition

	Event
		EName   Wind
		Type	start 
		At	Root

		Bhvr	:wirlpoolSpinInnerCore.bhvr

		geom	FX_TmpCtrlWisp01
		LifeSpan	50

	End
End

Condition
	On	cycle
	Time	10

	Event
		EName   Wind
		Type	start 
		At	Root

		Bhvr	:wirlpoolSpinInnerCore.bhvr

		geom	FX_TmpCtrlWisp01
		LifeSpan	50
	End
End

##########################################################
##Diffuse
##########################################################

Condition

	Event
		EName   Wind
		Type	start 
		At	Root

		Bhvr	:wirlpoolSpinInnerCore_Diffuse.bhvr

		geom	FX_Whirlpool_Diffuse
		LifeSpan	50

	End
End

Condition
	On	cycle
	Time	10
	Chance	.8

	Event
		EName   Wind
		Type	start 
		At	Root

		Bhvr	:wirlpoolSpinInnerCore_Diffuse.bhvr

		geom	FX_Whirlpool_Diffuse
		LifeSpan	45
	End
End

##################################################################
##Core
#########################################

Condition

	Event
		EName   Wind
		Type	start 
		At	Root
		
		Part	:waterCoreDifuseFlashes.part
		Part	:waterCoreBrightFlashes.part
		Part	:waterCoreWaterDiffuse.part
		Part	:waterCoreMist.part
		Part	:waterCoreWater.part
		Part	:waterCoreMistSmall.part

	End
End


##################################################################
##splats
#########################################

#Condition
#
#	Event
#		EName   Wind
#		Type	posit 
#		At	Root
#		
#		Bhvr	:waterRingsSplatShrink.bhvr
#		Splat	electricring1copy
#		LifeSpan	20
#
#	End
#End

Condition
	On	cycle
	Time	20
#	Chance	.8

	Event
		EName   Wind
		Type	posit 
		At	Root
		
		Bhvr	:waterRingsSplatShrink.bhvr
		Splat	electricring1copy
		LifeSpan	20

	End
End

Condition
	On	cycle
	Time	7
	Chance	.7

	Event
		EName   Wind
		Type	posit 
		At	Root
		
		Bhvr	:waterRingsSplatShrinkfast.bhvr	
		Splat	electricring1copy
		LifeSpan	20

	End
End

End		