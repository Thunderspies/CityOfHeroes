#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			EName MainAnchor
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.2 0
			End
			Part :Head_Trail_Large.part
			Part :Head_Trail_Large_Solid.part
		End

		Event
			Type SetLight
			EName FadeToBlack
			Bhvr :Entity_Darken_Fade.bhvr
			LifeSpan 40
		End

	End

	Condition
		On Cycle
		Time 2

		Event
			Type 	SetLight
			EName 	Black
			Bhvr 	:Entity_Darken.bhvr
			LifeSpan 25
		End

	End

	Condition
		On TriggerBits
		TriggerBits MALE
		DoMany 1

		Event
			At Head
			Type Local
			ChildFx :CHILD_Possess_Eyes_Male.fx
			EName EyeGlows_Male
			Flags OneAtATime
		End

	End

	Condition
		On TriggerBits
		TriggerBits FEMALE
		DoMany 1

		Event
			At Head
			Type Local
			ChildFx :CHILD_Possess_Eyes_Female.fx
			EName EyeGlows_Female
			Flags OneAtATime
		End

	End

	Condition
		On TriggerBits
		TriggerBits HUGE
		DoMany 1

		Event
			At Head
			Type Local
			ChildFx :CHILD_Possess_Eyes_Huge.fx
			EName EyeGlows_Huge
			Flags OneAtATime
		End

	End

	#Condition
		#On TriggerBits
		#TriggerBits BEAST
		#DoMany 1

		#Event
			#At Head
			#Type Local
			#ChildFx :CHILD_Possess_Eyes_Huge.fx
			#EName EyeGlows_Huge
			#Flags OneAtATime
		#End

	#End

End
