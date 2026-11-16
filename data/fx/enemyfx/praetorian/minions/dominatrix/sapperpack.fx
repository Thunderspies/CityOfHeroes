#########################################################
##	forceBubble
########################################################
FxInfo



########################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	Pack
		Type	local
		At	Back
		Geom	Malta_Sapper_Pack	
	End

	Event
		EName 	Offset
		Type	local
		At	Back
		Bhvr	:SapperPackOffset.bhvr
	
	End
	
	Event
		EName 	JetL
		Type	local 
		At	Offset
		Part1	EnemyFX/SkyRaiders/JetPackThrusterGlowBlue.part
		Part2	EnemyFX/SkyRaiders/JetPackThrusterCenterBlue.part
		#Part3	EnemyFX/SkyRaiders/JetPackThrustercore.part
		Part4	EnemyFX/SkyRaiders/JetPackThrusterTrail.part
		Part5	EnemyFX/SkyRaiders/JetPackThrusterGases.part
		Sound jetpack2_loop 60 60 .22
	End
	

#	Event
#		EName 	magnet1
#		Type	local
#		At	Offset
#		Part1	EnemyFX/SkyRaiders/Smoke.part
#		#Part2	EnemyFX/SkyRaiders/WingLightCenterBlue.part
#		
#	End

#	Event
#		EName 	JetR
#		Type	local
#		At	Offset
#		Part1	EnemyFX/SkyRaiders/WingLightStrip.part
#		PMagnet magnet2
#	End
#
#	Event
#		EName 	magnet2
#		Type	local
#		At	Prime
#		 AltPiv 6
#		Part1	EnemyFX/SkyRaiders/Smoke.part
#				
#	End

End

End

