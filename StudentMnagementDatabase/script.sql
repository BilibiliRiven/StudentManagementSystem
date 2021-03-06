SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
IF NOT EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].[tb_student_info]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
BEGIN
CREATE TABLE [dbo].[tb_student_info](
	[stu_id] [int] IDENTITY(1,1) NOT NULL,
	[stu_tel] [varchar](50) NULL CONSTRAINT [DF__tb_studen__stu_t__7E6CC920]  DEFAULT ('0000000000'),
	[stu_sex] [int] NOT NULL,
	[stu_del] [bit] NOT NULL,
	[stu_tag] [datetime] NOT NULL,
	[stu_name] [varchar](50) NULL,
 CONSTRAINT [pk_stuid] PRIMARY KEY CLUSTERED 
(
	[stu_id] ASC
) ON [PRIMARY]
) ON [PRIMARY]
END
