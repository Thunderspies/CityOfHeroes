#########################################################
##	forceBubble
########################################################

FxInfo

Flags InheritAlpha DontSuppress

########################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Pack
		Type	local
		At	Back
		Bhvr	Behaviors/GenericParticleFade4.bhvr
		Geom	Malta_Sapper_Pack
	End

End

Condition
	On	TriggerBits
	TriggerBits	jump
	DoMany		1

	Event
		EName 	Offset
		Type	local
		At	Back
		While	jump
		Flags	OneAtATime

		Bhvr	:SapperPackOffset.bhvr

	End

	Event
		EName 	JetL
		Type	local
		At	Offset
		While	jump
		Flags	OneAtATime

		Bhvr	Behaviors/GenericParticleFadeQuicker.bhvr
		Part1	EnemyFX/SkyRaiders/JetPackThrusterGlowBlue.part
		Part2	EnemyFX/SkyRaiders/JetPackThrusterCenterBlue.part
		#Part3	EnemyFX/SkyRaiders/JetPackThrustercore.part
		Part4	EnemyFX/SkyRaiders/JetPackThrusterTrail.part
		Part5	EnemyFX/SkyRaiders/JetPackThrusterGases.part
		Sound jetpack2_loop 60 60 .22
	End


End

End

