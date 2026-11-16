#########################################################
## Endocronic Blast - Hit FX
#########################################################

FxInfo

Flags    InheritAlpha

LifeSpan 150

## BEAM ######################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Waist
		Sound mitoblast1 70 70 .7
	End		

	Event
		EName	LookAtHookup
		Type	Posit
		At	Chest
		LookAt	Target
	End		


	Event
		Type	Local
		At	LookAtHookup
		BhvrOverride
			PositionOffset	0 0 2.5
		End
		Part2	:Beam_Core01.part
		Part3	:Beam_Core02.part
		Part4	:Beam_Core03.part
		Part4	:Beam_Core04.part
		Part2	:Beam_Glow01.part
		POther	Target
	End		
End

## PULSE ######################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Start 
		At	Waist
		Bhvr	behaviors/powers/coldcontrol/iceblast.bhvr
		BhvrOverride
			TrackRate		4
		End
		Magnet	Target
		LookAt	Target
		Part	:Pulse_Core01.part
		#Part	:Pulse_Core02.part
		Part	:Pulse_Tail01.part
	End
End

## HIT FX #######################################################

Condition
	On 	Time
	Time 	0
			
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Hamidon_Mito_Attack_Burst_01.part
		Sound mitohit1 60 60 .7
		Lifespan	3
	End
	
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Hamidon_Mito_Attack_Burst_Bits_01.part
		Lifespan	7
	End
	
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Hamidon_Mito_Attack_Burst_02.part
		Lifespan	40
	End
End

#########################################################

End