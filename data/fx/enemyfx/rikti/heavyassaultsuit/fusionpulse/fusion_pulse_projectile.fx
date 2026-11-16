#########################################################
## Rikti Heavy Assault Suit - Fusion Pulse Projectile ChildFX	
#########################################################

FxInfo

## PROJECTILE CHILD FX #######################################################

Condition

	Event
		EName 	ProjectileA
		Type	start 
		At	Origin
		Geom	LightDummy2
		bhvr	behaviors/spin3b.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	s1
		Type	local 
		At	ProjectileA
		altpiv	1
		Part1	WEAPONFX\RiktiEnergyStreamer2.part
	End

	Event
		EName 	s2
		Type	local 
		At	ProjectileA
		altpiv	2
		Part1	WEAPONFX\RiktiEnergyStreamer2.part
	End

	Event
		EName 	ProjectileB
		Type	start 
		At	Origin
		Geom	LightDummy2
		bhvr	behaviors/spin3bOpposite.bhvr
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	s1
		Type	local 
		At	ProjectileB
		altpiv	1
		Part1	WEAPONFX\RiktiEnergyStreamer2.part
	End

	Event
		EName 	s2
		Type	local 
		At	ProjectileB
		altpiv	2
		Part1	WEAPONFX\RiktiEnergyStreamer2.part
	End
End

#########################################################

End				