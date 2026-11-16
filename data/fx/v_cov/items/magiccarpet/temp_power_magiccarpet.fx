#
FxInfo


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	MagicCarpet_Equip_01 100 100 .8
	End
End


	Condition
		On Time
		Time 0
		DoMany 1

		Event
			Anim magiccarpet
			At Mystic
			Type Local
			EName CarpetAnchor
			Flags OneAtATime
			BhvrOverride
				PositionOffset 0 0 0
				Scale 1.5 1.5 1.5
			End
		End

		Event
			AnimPiv Head
			At CarpetAnchor
			Type Local
			EName GlowVert
			Flags OneAtATime
			BhvrOverride
				PositionOffset -0.85 0.4 0.93
				StartColor 255 250 15
			End
			Geom MagicCarpet_Lamp_GlowPlane2
		End

		Event
			AnimPiv Head
			At CarpetAnchor
			Type Local
			EName GlowFlat
			Flags OneAtATime
			BhvrOverride
				PositionOffset -0.85 0.0 0.93
				StartColor 255 250 15
				#PyrRotate 90 0 0
			End
			Geom MagicCarpet_Lamp_GlowPlane3
		End

		Event
			AnimPiv Head
			At CarpetAnchor
			Type Local
			EName GlowInner
			Flags OneAtATime
			Geom MagicCarpet_Lamp_Glow
		End

	End

#####TRAIL

	Condition
		On TRIGGERBITS
		Triggerbits FORWARD
		DoMany 1

		Event
			AnimPiv Head
			At CarpetAnchor
			Type Local
			EName Streak
			Flags OneAtATime
			While	FORWARD
			Childfx :MagicCarpet_Trail_Forward.fx
		End
	End

	Condition
		On TRIGGERBITS
		Triggerbits BACKWARD
		DoMany 1

		Event
			AnimPiv Back
			At CarpetAnchor
			Type Local
			EName Streak
			Flags OneAtATime
			While	BACKWARD
			Childfx :MagicCarpet_Trail_Forward.fx
		End
	End

	Condition
		On TRIGGERBITS
		Triggerbits STEPLEFT
		DoMany 1

		Event
			AnimPiv Chest
			At CarpetAnchor
			Type Local
			EName Streak
			Flags OneAtATime
			While	STEPLEFT
			BhvrOverride
				PositionOffset 2 0 0
			End
			Childfx :MagicCarpet_Trail_Sideways.fx
		End
	End

	Condition
		On TRIGGERBITS
		Triggerbits STEPRIGHT
		DoMany 1

		Event
			AnimPiv Chest
			At CarpetAnchor
			Type Local
			EName Streak
			Flags OneAtATime
			While	STEPRIGHT
			BhvrOverride
				PositionOffset -2 0 0
			End
			Childfx :MagicCarpet_Trail_Sideways.fx
		End
	End

End
