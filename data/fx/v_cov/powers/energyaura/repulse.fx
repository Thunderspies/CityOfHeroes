#########################################################
##	template

FxInfo

#LifeSpan	20

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound kinetic4 80 80 .84
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	cycle
	Time	10

	Event	
		Type	Local
		At	Root	
		BHVR	:RepulseScaleRotated.bhvr
		GEOM	RepulseWave
		LifeSpan	12
	End

	Event	
		Type	Local
		At	Root	
		BHVR	:RepulseScaleRotatedOpposite.bhvr
		GEOM	RepulseWaveInvert
		LifeSpan	12
	End

End

Condition
	On	cycle
	Time	10
	#Chance	.25

	Event	
		Type	Local
		At	Root	
		BHVR	:RepulseScale.bhvr
		GEOM	RepulseWave
		LifeSpan	30
	End

End

#Condition
#	On	cycle
#	Time	7
#	
#	Event	
#		Type	Local
#		At	Root
#		BHVR	:RepulseRingsplat.bhvr
#		
#		Splat	Coffee_Ring.tga	#Generic_Ring.tga
#		LifeSpan	5
#	End
#
#End
#
#Condition
#	On	cycle
#	Time	13
#
#	Event	
#		Type	Local
#		At	Root
#		BHVR	:RepulseRingsplat.bhvr
#		
#		Splat	Coffee_Ring.tga	#Generic_Ring.tga
#		LifeSpan	5
#	End
#
#End
#
End