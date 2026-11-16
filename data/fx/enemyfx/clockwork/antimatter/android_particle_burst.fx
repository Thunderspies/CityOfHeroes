#############################################################
## Android_Particle_Stunner.fx
#############################################################

FxInfo
Lifespan 100

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		sound Particle_Burst 200 200 .8
	End

	Event
		Ename	MuzzleEffects
		Type	Local
		At	C_Hair
		Part	:Muzzle_ElectricSmoke.part
		Part	:Muzzle_Strobe_Core.part
		Part	:Muzzle_Strobe_Glow_Blue.part
		Part	:Muzzle_Electricity.part
		Lifespan 70
	End
End

#############################################################################

Condition
	On	Time
	Time	70

	Event
		Ename	Prime
		Type	Start
		At 	C_Hair
		BhvrOverride
			TrackRate	3.5
		End
		Part	:Projectile_Particle_TrailCore.part
		Part	:Projectile_Particle_Head.part
		Part	:Projectile_Particle_Tail.part
		Part	:Projectile_Particle_Tail_Stretched.part

		Magnet	Target
		LookAT	Target

		Lifespan	200
	End
End

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End
#############################################################################

End