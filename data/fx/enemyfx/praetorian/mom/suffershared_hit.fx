#
FxInfo

	LifeSpan 90


###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	MoM_Suffer_Hit_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName cones
			Part ENEMYFX\Praetorian\MoM\Cone_Rise2.part
			Part ENEMYFX\Praetorian\MoM\Cone_Rise.part
			LifeSpan 3
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Chest
			Type Local
			EName splode
			Part ENEMYFX\Praetorian\MoM\HitFlash.part
			Part ENEMYFX\Praetorian\MoM\HitSparks.part
			Part ENEMYFX\Praetorian\MoM\Suffer_Smoke_Lingering.part
			Part ENEMYFX\Praetorian\MoM\Blobby.part
			LifeSpan 5
		End

	End

	#Condition
		#On Time
		#Time 0

		#Event
			#At Root
			#Type Local
			#
			#ChildFx :SufferShared.fx
			#EName test
			#LifeSpan 60
		#End

	#End

End
