#########################################################
## Blue Mitochondria - Mez attack
########################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

#	Event
#		Type	local
#		At	Waist
#		Part	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_Mez\Hamidon_Mito_Mez_Core.part
#		Part	ENEMYFX\DevouringEarth\Hamidon\Power_Mito_Mez\Hamidon_Mito_Mez_Twinkles.part
#		#Sound	HamidonElecWindup 80 70 .66
#		Lifespan 60
#	End

	Event
		EName	TargetHookup
		Type	Posit
		At	T_Chest
		LookAt	Target
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Part	Scripted\RomulusNictus\Nictus_Beam\NictusBeam_Black01.Part
		Part	Scripted\RomulusNictus\Nictus_Beam\NictusBeam_Blue01.Part
		Part	Scripted\RomulusNictus\Nictus_Beam\NictusBeam_Violet01.Part
		Part	Scripted\RomulusNictus\Nictus_Beam\NictusBeam_Glow.part
		POther	TargetHookup
	End
End

#############################################################

Condition
	On 	Cycle
	Time 	6

	Event
		Type	Local
		At	Origin
		Part	Scripted\RomulusNictus\Nictus_Beam\NictusBeam_Black01.Part
		Part	Scripted\RomulusNictus\Nictus_Beam\NictusBeam_Blue01.Part
		Part	Scripted\RomulusNictus\Nictus_Beam\NictusBeam_Violet01.Part
		Part	Scripted\RomulusNictus\Nictus_Beam\NictusBeam_Glow.part
		POther	TargetHookup
		Lifespan 1
	End
End

Condition
	On 	Cycle
	Time 	4
	Chance	0.25

	Event
		Type	Local
		At	Origin
		Part	Scripted\RomulusNictus\Nictus_Beam\NictusBeam_Blue02.Part
		Part	Scripted\RomulusNictus\Nictus_Beam\NictusBeam_Glow_Blue.part
		POther	TargetHookup
		Lifespan 60
	End
End

Condition
	On 	Cycle
	Time 	4
	Chance	0.25

	Event
		Type	Local
		At	Origin
		Part	Scripted\RomulusNictus\Nictus_Beam\NictusBeam_Violet02.Part
		Part	Scripted\RomulusNictus\Nictus_Beam\NictusBeam_Glow_Violet.part
		POther	TargetHookup
		Lifespan 60
	End
End

Condition
	On 	Cycle
	Time 	4
	Chance	0.25

	Event
		Type	Local
		At	Origin
		Part	Scripted\RomulusNictus\Nictus_Beam\NictusBeam_Black02.Part
		Part	Scripted\RomulusNictus\Nictus_Beam\NictusBeam_Glow_Black.part
		POther	TargetHookup
		Lifespan 60
	End
End

########################################################

End