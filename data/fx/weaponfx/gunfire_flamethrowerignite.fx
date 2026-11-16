#############################################################
## Gunfire_FlameThrowerIgnite.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	34

	Event
		ENAME   p2
		Type	Local
		At	WepR
		Geom	FlameThrower
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv	1
		Part1	:FlameThrowerFlame.part
		Part2	WEAPONFX/TorchflamethrowerB.part
		Part	:FireSpecksSpit.part
		Part	:FireSpecksFalling.part
		Part	:FireSpit2.part
		Part	:FireSpitStart.part
		Sound flamethrower 120.0 120.0 .98
		Lifespan 30
	End

	Event
		EName 	Blast2
		Type	local
		At	p2
		Altpiv	1
		Part1	WEAPONFX/TorchflamethrowerA.part
		Lifespan 20
	End
End

#############################################################

Condition
	On 	Time
	Time 	34

	Event
		EName 	Prime
		Type	start
		At	C_Neck
		Bhvr	Behaviors\Fastprojectile.bhvr
		BhvrOverride
			TrackRate	0.75
		End
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End