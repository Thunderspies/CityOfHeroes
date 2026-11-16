#############################################################
## Assault Rifle - Flamethrower Attack FX
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName 	Blast1
		Type	local
		At	C_Neck
		Part1	:FlameThrowerFlame.part
		Part	:FireSpecksSpit.part
		Part	:FireSpecksFalling.part
		Part	:FireSpit2.part
		Part	:FireSpitStart.part
		Sound flamethrower2 120.0 120.0 .95
		Lifespan 61
	End

	Event
		EName 	Blast2
		Type	local
		At	C_Neck
		Part1	WEAPONFX/TorchflamethrowerA.part
		Lifespan 41
	End

	Event
		EName 	Blast3
		Type	local
		At	C_Neck
		Part2	:TorchflamethrowerB.part
		Lifespan 55
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

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