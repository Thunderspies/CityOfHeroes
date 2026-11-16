#                                                          
FxInfo

	LifeSpan 60


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Start
			CEvent WORLD\tunnelsDebrisConcrete_SFX.fx
			#CThresh 0.00001
			Part :CHILD_RockBreak_Dust.part
			Part :CHILD_RockBreak.part
			LifeSpan 500
			LifeSpanJitter 90
		End

		Event
			At Origin
			Type Start
			#CThresh 0.00001
			Part :CHILD_RockBreak_Dust.part
			Part :CHILD_RockBreak.part
			LifeSpan 500
			LifeSpanJitter 90
		End

	End

End
