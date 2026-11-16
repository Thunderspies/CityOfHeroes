#########################################################
## Arachnos PTT - Power Buff (Red)
#########################################################

FxInfo

LifeSpan	120

#########################################################

## Dummy Node ##
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	DummyNode
#		Type	Start 
#		At	Head
#		BhvrOverride
#			Spin			0 0 0.25
#		End
#
#		Magnet	Target
#		LookAt	Target
#
#	End
#
#End
#
#
### Offset Node ##
#Condition
#	On 	Cycle
#	Time 	1
#
#	Event
#		EName 	DummyNodeOffset
#		Type	Local
#		At	DummyNode
#
#		BhvrOverride
#			PositionOffset		0 1.5 0
#			InitialVelocity		0.0 0.0 1.0
#		End
#
#		Part1	:Streakers_Green.part
#
#		Magnet	Target
#		LookAt	Target
#		
#		Lifespan 30
#
#	End
#	
#
#End

## Helix ##
Condition
	On 	Time
	Time 	0

	Event ## GEO Strand
		EName 	Prime1
		Type	Start 
		At	Head
		Bhvr	:behaviors\Beam.bhvr
		
		geom	GEO_ArachnosPTTStrand_Red

		Magnet	Target
		LookAt	Target

		LifeSpan		90
	End
End

## Beam ##
Condition
	On 	Time
	Time 	0

	Event
		EName	prime2
		Type	start
		At	Head
		Part1	:Beam_Red.part
		
		PMagnet	Target
	End
End

## Lingering trail ##
Condition
	On 	Time
	Time 	0

	Event
		EName	prime3
		Type	start
		At	Head
		Bhvr	:behaviors\LingeringTrail.bhvr
		Part1	:LingeringPlasma1_Red.part
		
		Magnet	Target
		LookAt	Target
	End
End

## Pulse ##
Condition
	On 	Time
	Time 	30

	Event
		EName	prime4
		Type	start
		At	Head
		Bhvr	:behaviors\Pulse.bhvr
		Part1	:LingeringPlasma2_Red.part
		Part2	:LingeringPlasma3_Red.part
		
		Magnet	Target
		LookAt	Target
	End
End

##############################################################################

#Condition
#	On	DummyNodeHit
#		
#	Event
#		Ename	DummyNode
#		Type	Destroy
#	End
#
#End
#
#Condition
#	On	StreakersHit
#		
#	Event
#		Ename	Streakers
#		Type	Destroy
#	End
#
#End
#

Condition
	On	Prime1Hit
		
	Event
		Ename	Prime1
		Type	Destroy
	End
End

Condition
	On	Prime2Hit

	Event
		Ename	Prime2
		Type	Destroy
	End
End

Condition
	On	Prime3Hit

	Event
		Ename	Prime3
		Type	Destroy
	End
End

Condition
	On	Prime4Hit

	Event
		Ename	Prime4
		Type	Destroy
	End
End


End	