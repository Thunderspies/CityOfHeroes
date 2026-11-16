#########################################################
## Rikti Pylon - Persistent FX
#########################################################

FxInfo

##########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	RedLight1
		Type	Local
		At	Col_R
		BhvrOverride
			PositionOffset	0.75 0 0
		End
		Part	:Light_RedGlow01.part
	End
	
#	Event
#		Type	Local
#		At	RedLight1
#		Part	:BlinkingLight_RedCore01.part
#		Part	:BlinkingLight_RedGlow01.part
#		Lifespan	15
#	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	RedLight2
		Type	Local
		At	Col_L
		BhvrOverride
			PositionOffset	-0.75 0 0
		End
		Part	:Light_RedGlow01.part
	End
	
#	Event
#		Type	Local
#		At	RedLight2
#		Part	:BlinkingLight_RedCore01.part
#		Part	:BlinkingLight_RedGlow01.part
#		Lifespan	15
#	End
End

Condition
	On 	Cycle
	Time 	30
	
	Event
		Type	Local
		At	RedLight1
		Part	:BlinkingLight_RedCore01.part
		Part	:BlinkingLight_RedGlow01.part
		Lifespan	15
	End
End

Condition
	On 	Cycle
	Time 	30
	
	Event
		Type	Local
		At	RedLight2
		Part	:BlinkingLight_RedCore01.part
		Part	:BlinkingLight_RedGlow01.part
		Lifespan	15
	End
End


## LIGHTS - BLUE ########################################################

Condition
	On 	Time
	Time 	61

	Event
		EName 	BlueLight1
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0 8 0
		End
		Part	:Light_BlueGlow01.part
	End
End


## BASE RINGS ########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	BaseRings
		Type	Start
		At	Hips
		BhvrOverride
			PositionOffset	0 7.75 0
		End
		Part	:Base_BlueRings01.part
	End
End

##########################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	BeamOffset1
#		Type	Local
#		At	Waist
#		BhvrOverride
#			PositionOffset	16 0 0
#		End
#	End
#	
#	Event
#		Type	Local
#		At	RedLight1
#		Part	:Beam_RedGlow1.part
#		Part	:Beam_RedCore1.part
#		pOther	BeamOffset1
#	End
#End
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	BeamOffset2
#		Type	Local
#		At	Waist
#		BhvrOverride
#			PositionOffset	-16 0 0
#		End
#	End
#	
#	Event
#		Type	Local
#		At	RedLight2
#		Part	:Beam_RedGlow1.part
#		Part	:Beam_RedCore1.part
#		pOther	BeamOffset2
#	End
#End

#########################################################

End