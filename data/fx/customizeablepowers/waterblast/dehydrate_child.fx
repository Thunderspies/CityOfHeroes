#
FxInfo

	LifeSpan 200

Input
	InpName	Hips
End

###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	WB_Dehydrate 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At T_Chest
			Type PositOnly
			EName Anchor
		End

		Event
			At Chest
			Type PositOnly
			LookAt Anchor
			Magnet Anchor
			PMagnet Anchor
			Bhvr :WaterTube.bhvr
			BhvrOverride
				TintGeom 1
				#Alpha 180
			End
			Part :Drain_Water_Spray.part
			Part :Drain_Water_Mist2.part
			Part :Drain_Water_Drips.part
			Part :Drain_Water_Mist.part
			Geom FX_WaterTube2_Add
			LifeSpan 60
		End

		Event
			At Chest
			Type Local
			Part :Hit_Streaks.part
			Part :Splash_Flash.part
			LifeSpan 2
		End

	End

	Condition
		On Time
		Time 10

		Event
			At T_Chest
			Type PositOnly
			Part :WaterBurst_DrawIn_Spray.part
			Part :WaterShot_GlowStreaks.part
			Part :Drain_DrawIn_GlowStreaks.part
			Part :Drain_DrawIn_Spray.part
			#Part :WaterBurst_DrawIn_Splatter.part
			LifeSpan 50
		End

	End

End
