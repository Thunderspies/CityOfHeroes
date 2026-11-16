#############################################################
## Android_Particle_Stunner.fx
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		sound Particle_Spray 200 200 .8
	End

	Event
		Ename	MuzzleEffects
		Type	PositOnly
		At	C_Hair
		Part	:Muzzle_ElectricSmoke.part
		Part	:Muzzle_Strobe_Core.part
		Part	:Muzzle_Strobe_Glow_Blue.part
		Part	:Muzzle_Electricity.part
		Lifespan 105
	End
End

#############################################################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Root
		ChildFX	ENEMYFX\Clockwork\antimatter\Child_ParticleSpray_ElectricalArcs.fx
		Lifespan 60
	End

#############################################################################

End